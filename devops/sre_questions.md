- https://www.youtube.com/watch?v=sL7h1rOn0K0&list=PLhqPDa2HoaAZcamYtXr-ijXBT-vcUBcNX
- https://awstip.com/sre-devops-interview-questions-linux-troubleshooting-1b8ffe82c16
- https://awstip.com/sre-devops-interview-questions-linux-troubleshooting-extended-c12cb5ded3b0
- https://github.com/mxssl/sre-interview-prep-guide?tab=readme-ov-file


# What happens internally when we type a command in the shell

When you type a command in the shell, the shell first parses the input, performing expansions (like variables, command substitution, and globbing), and checks whether the command is a built-in, function, or an external executable. If it’s external, the shell searches for it in the directories listed in the $PATH environment variable. Once located, the shell creates a child process using fork(), and the child process replaces itself with the executable using exec(). Before execution, the shell sets up any necessary input/output redirection. The parent shell then waits for the child to finish unless the command is run in the background, and finally returns control to the user after collecting the command's exit status.

## What Happens When You Type `ps` in Linux

1. **Shell Input**
   - You type `ps` and press Enter.
   - Shell parses it, finds the binary via `$PATH`.

2. **Process Fork and Exec**
   - Shell calls `fork()` → child process created.
   - Child calls `execve("/bin/ps", ["ps"], envp)` → replaces shell with `ps`.

3. **Kernel: execve Syscall**
   - `execve` triggers a `syscall` (number 59 on x86_64).
   - Kernel loads the ELF binary, sets up stack, arguments, and instruction pointer.

4. **Program Starts**
   - Execution begins at `_start`, then calls `main()`.

5. **ps Reads Process Info**
   - Reads `/proc/[pid]/stat`, `/proc/[pid]/cmdline`, etc.
   - Uses syscalls like `open`, `read`, `getdents64`, `close`.

6. **Output**
   - Formats process info and writes to stdout using `write(1, ...)`.

7. **Exit**
   - Calls `exit()` → kernel handles cleanup via `exit_group` syscall (231).

---

## What happens when you call `nohup program&`

- `nohup` ignores SIGHUP so the program isn't terminated when the terminal closes  
- `&` runs the program in the background  
- the command becomes immune to hangups (e.g. terminal logout or SSH disconnect)  
- stdout and stderr are redirected to `nohup.out` if not explicitly redirected  
- the shell immediately returns control to the user  
- the background process gets a new PID, shown by the shell  
- it keeps running in the background even after logout  
- input is detached from the terminal (stdin is usually set to `/dev/null`)  
- job control features like `jobs`, `fg`, and `bg` can still be used in the current shell session while it's active

# Linux process/threads

## signal handling
- In Linux, a signal is a kernel mechanism used to asynchronously notify a process that an event has occurred—such as an interrupt, fault, or termination request. Common examples include SIGINT (interrupt), SIGTERM (termination), and SIGKILL (force kill).
- When a signal is sent (e.g., via kill or a hardware fault), the kernel marks it as pending for the target process. If the signal is unblocked, the kernel delivers it by interrupting the process's execution. Each signal has a default disposition—such as terminating the process or generating a core dump—but a process can override this using signal handlers (signal() or sigaction()), or block signals via signal masks to delay handling during critical sections.
- Some signals (like real-time signals) are queued, but standard signals are not—only one pending instance is tracked. Signals are crucial in SRE for managing long-running daemons, handling graceful shutdowns, debugging crashes (e.g., via SIGSEGV), or implementing monitoring hooks using SIGUSR1/2.

**Can be handled (most common):**
- `SIGINT` (Ctrl+C)
- `SIGTERM` (kill default)
- `SIGHUP` (terminal closed)
- `SIGUSR1`, `SIGUSR2` (user-defined)
- `SIGALRM`, `SIGCHLD`, `SIGPIPE`, `SIGTSTP`, `SIGCONT`, `SIGQUIT`

**Cannot be caught or ignored:**
- `SIGKILL` (signal 9) — always terminates the process
- `SIGSTOP` (signal 19) — always suspends the process

**Notes:**
- `SIGSEGV`, `SIGFPE`, `SIGILL`, `SIGBUS` can be caught but should be used carefully (used for crashes/faults)
- Use `sigaction()` to handle signals robustly
- Masking or ignoring critical signals like `SIGKILL`/`SIGSTOP` is not possible by design

## thread vs process
- **Process**: A process is an independent program in execution with its own memory space, file descriptors, and system resources. Each process has a unique **PID** (Process ID) and is managed by the kernel. Processes don’t share memory unless explicitly set up via inter-process communication (IPC).

- **Thread**: A thread is a lightweight unit of execution within a process. Multiple threads in the same process share the same memory and resources, allowing faster communication and lower overhead than processes. Threads are managed by the **scheduler**, just like processes, and are identified by **TIDs** (Thread IDs).

### Key Differences:
| Aspect         | Process                          | Thread                             |
|----------------|----------------------------------|-------------------------------------|
| Memory         | Separate memory space            | Shared memory within the process    |
| Communication  | Needs IPC mechanisms             | Can communicate via shared variables |
| Overhead       | Higher (more isolation)          | Lower (lighter weight)              |
| Crash Impact   | Crash doesn't affect others      | Crash can bring down the process    |
| Linux Creation | `fork()`, `exec()`               | `pthread_create()` or via clone()   |

In Linux, threads are implemented using the `clone()` system call and are treated as **lightweight processes** (tasks), which is why Linux sometimes refers to both threads and processes collectively as “tasks.” Tools like `ps`, `top`, or `htop` can show both processes and threads depending on the flags used.

## Linux process hierarchy

- All processes form a tree rooted at `init` or `systemd` (PID 1)
- Each process has a parent (PPID) and may have children
- Child processes are created using `fork()` or `clone()`
- The kernel tracks these relationships to manage resources and signals
- Use `ps -ef`, `pstree`, or `top` to view the hierarchy

## Zombie process

- A zombie is a child process that has exited but still has an entry in the process table
- Happens when the parent hasn’t called `wait()` to collect its exit status
- Zombies appear with status `Z` in `ps` or `top`
- If the parent exits, `init` adopts and reaps the zombie
- Too many zombies can exhaust PID space and indicate a bug in the parent process

## Linux NICE values

- A **nice value** determines the *priority* of a user-space process during CPU scheduling.
- Range: **-20 (highest priority)** to **+19 (lowest priority)**. Default is **0**.
- A *lower nice value* means the process is treated as more important and gets more CPU time.
- Only the **root user** can assign negative nice values (raise priority).
- You can set the nice value at process launch with `nice -n <value> <command>`, or change it for a running process with `renice`.
- The kernel uses the **nice value as input to the scheduler**, which also considers other factors like I/O wait, sleep time, etc.

Here are the main Linux commands to view and change nice values:

- **Check nice value of running processes**  
  `ps -eo pid,comm,nice`  
  or  
  `top` (look under the **NI** column)

- **Start a process with a specific nice value**  
  `nice -n 10 some_command`  
  (default is 0; positive values lower priority)

- **Change nice value of a running process**  
  `renice -n 5 -p <PID>`  
  (only root can decrease the nice value, e.g., from 10 to 0)

- **Check effective scheduling priority (more detail)**  
  `chrt -p <PID>`  
  (for real-time and other policies, not just nice)

Example:  
If two CPU-bound processes are running, one with nice 0 and one with nice 10, the one with nice 0 will get more CPU time.

## Pipes

- In Linux, pipes are used for inter-process communication (IPC), allowing data to flow from one process to another, typically in a unidirectional stream. Here's how they work:
- A pipe is created using the `pipe()` system call, which returns two file descriptors:
  - `fd[0]` for reading
  - `fd[1]` for writing
- In a shell, using `|` (pipe symbol), like `ls | grep txt`, sets up a pipe between `ls` and `grep`
- The shell:
  - Creates a pipe
  - Forks two child processes
  - Redirects `stdout` of the first command to `fd[1]` (write end)
  - Redirects `stdin` of the second command to `fd[0]` (read end)
  - Closes unused ends in each process
- Data written to `fd[1]` by the producer process is buffered by the kernel and can be read from `fd[0]` by the consumer process
- Pipes are unidirectional: to get bidirectional communication, two pipes are needed
- `pipe()` is typically used in low-level C code; in the shell, pipes are created implicitly using `|`
- Pipes are anonymous (unnamed) unless explicitly created using named pipes (`mkfifo`) for communication between unrelated processes
- named pipes (FIFOs):
  - created with `mkfifo` or `mknod <name> p`
  - visible in filesystem, used by unrelated processes
  - `echo hi > mypipe` (writer blocks until reader connects)
  - `cat < mypipe` (reader blocks until writer writes)
- buffering:
  - kernel buffer, usually 64 KB
  - writer blocks if buffer is full
  - reader blocks if buffer is empty
  - no user-level flushing needed
- blocking behavior:
  - default mode is blocking
  - non-blocking with `fcntl(fd, F_SETFL, O_NONBLOCK)`
  - `read()` returns -1 with `EAGAIN` if no data
  - `write()` returns -1 with `EAGAIN` if buffer full
  - use with `select()`, `poll()`, or `epoll()` for multiplexing

## fork and exec

- `fork()` creates a **new child process** by duplicating the current process (parent). Both processes continue executing from the next line after `fork()`.
- Return values of `fork()` help distinguish:
  - `>0`: in parent, return is child's PID  
  - `0`: in child  
  - `<0`: error in creating process  
- After `fork()`, the child is often replaced with a new program using `exec()`.
- `exec()` loads a new binary into the current process's memory, replacing its code, stack, heap, and data.
- It does **not return** if successful—execution continues in the new program.
- If `exec()` fails, control returns to the child process.
- `fork()` + `exec()` is commonly used to run new programs (e.g., in shells).
- File descriptors, environment variables, and signal dispositions are preserved across `exec()` unless specifically cleared.

Example flow:
- Shell calls `fork()` → child created  
- In child: `exec("/bin/ls")` → now running `ls`  
- In parent: `wait()` for child to finish  

Used for:
- Spawning subprocesses  
- Implementing job control in shells  
- Isolating new programs from current code

- **Memory**:  
  - On `fork()`, the entire memory space is logically duplicated (stack, heap, data, code), but most OSes use **copy-on-write** to avoid physically copying until modified.
  - On `exec()`, the process's memory is **replaced** with the new program’s memory layout.

- **File descriptors**:  
  - On `fork()`, file descriptors are **copied by reference** — parent and child share the same underlying file table entries.
  - On `exec()`, file descriptors remain open **unless** they have the `FD_CLOEXEC` flag set, which causes them to be closed during `exec()`.

Summary:
- `fork()`: memory is logically copied (copy-on-write), file descriptors shared  
- `exec()`: memory is replaced, file descriptors preserved unless `FD_CLOEXEC`

# Whats an inode

An **inode** (index node) in Linux is a data structure used by file systems (like ext4, xfs, etc.) to store **metadata about a file or directory** — but **not** the file’s name or its actual data.

# soft/hard link

In Linux, a hard link is an additional directory entry that points directly to the inode (the actual data) of a file, meaning multiple filenames can refer to the same content, and deleting one does not remove the data as long as another hard link exists. In contrast, a soft link (or symbolic link) is a pointer to the file path, similar to a shortcut; if the target file is deleted, the symlink becomes broken. Hard links can't span filesystems or point to directories, while soft links can, making them more flexible but less robust if the target path changes.

# User and Kernel space 

“In Linux and most modern operating systems, memory is divided into two main regions: Kernel Space and User Space.

Kernel Space is where the core of the operating system runs — including things like process scheduling, memory management, and hardware drivers. It has full access to the system’s hardware and resources. Code running in kernel space operates in a privileged mode, so a fault here can impact the entire system.

User Space, on the other hand, is where all user applications run. These applications don’t have direct access to hardware or kernel memory. Instead, they communicate with the kernel through controlled interfaces, primarily system calls. This separation enforces security and stability — for example, if an application crashes in user space, it doesn’t bring down the whole OS.

This design ensures that bugs or vulnerabilities in user applications are contained and cannot compromise the core of the system.”**

# **Linux Boot Process (Interview Format):**

1. **BIOS/UEFI Initialization**:  
   When the system powers on, the **BIOS or UEFI** firmware performs POST (Power-On Self Test) and initializes hardware. It locates the bootable device and loads the bootloader into memory.

2. **Bootloader (e.g., GRUB)**:  
   The **bootloader** is responsible for loading the kernel. GRUB (GRand Unified Bootloader) presents a boot menu (if configured) and loads the selected Linux kernel and the initial RAM disk (**initrd/initramfs**).

3. **Kernel Loading**:  
   The **Linux kernel** is decompressed and loaded into memory. It initializes hardware drivers, mounts the initial RAM filesystem, and sets up memory management and process scheduling.

4. **Initramfs and Init Process**:  
   The **initramfs** contains scripts and tools needed for early boot (e.g., finding and mounting the root filesystem). Once ready, it hands control to **PID 1**, the **`init`** system (e.g., `systemd`, `SysVinit`, or `upstart`).

5. **System Initialization**:  
   The init system sets up all user-space services and targets (e.g., network, file systems, login services). With `systemd`, this involves processing unit files to start services in parallel.

6. **Login Prompt**:  
   Finally, a **login prompt or graphical display manager** is shown, allowing user interaction.

# Virtual memory

**Virtual memory** is a memory management technique used by operating systems to give the illusion of a large, continuous memory space to processes, even if the physical RAM is limited.

### Key Points:
- **Abstraction**: Each process gets its own virtual address space, isolated from others.
- **Paging**: Memory is divided into fixed-size blocks (pages), which can be moved between physical RAM and disk (usually in a swap file or partition).
- **Benefits**:
  - Allows programs to use more memory than physically available.
  - Improves isolation and security between processes.
  - Simplifies memory allocation for applications.

### How It Works:
- When a process accesses memory, the virtual address is translated to a physical address by the **Memory Management Unit (MMU)** using **page tables**.
- If the page is not in RAM, a **page fault** occurs and the OS loads it from disk (swap) into memory.

In short, virtual memory allows efficient and secure use of memory by abstracting physical memory and using disk as an extension when needed.
