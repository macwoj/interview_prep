- https://www.youtube.com/watch?v=sL7h1rOn0K0&list=PLhqPDa2HoaAZcamYtXr-ijXBT-vcUBcNX
- https://awstip.com/sre-devops-interview-questions-linux-troubleshooting-1b8ffe82c16
- https://awstip.com/sre-devops-interview-questions-linux-troubleshooting-extended-c12cb5ded3b0
- https://github.com/mxssl/sre-interview-prep-guide?tab=readme-ov-file


# What happens internally when we type a command in the shell

When you type a command in the shell, the shell first parses the input, performing expansions (like variables, command substitution, and globbing), and checks whether the command is a built-in, function, or an external executable. If it’s external, the shell searches for it in the directories listed in the $PATH environment variable. Once located, the shell creates a child process using fork(), and the child process replaces itself with the executable using exec(). Before execution, the shell sets up any necessary input/output redirection. The parent shell then waits for the child to finish unless the command is run in the background, and finally returns control to the user after collecting the command's exit status.

# thread vs process

In Linux:

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