- https://www.youtube.com/watch?v=sL7h1rOn0K0&list=PLhqPDa2HoaAZcamYtXr-ijXBT-vcUBcNX

# What happens internally when we type a command in the shell

### 🧠 1. **Read and Parse the Input**
- The shell reads the line you typed.
- It parses the input to:
  - Separate the command and arguments.
  - Perform shell expansions like:
    - **Variable expansion**: `$HOME` → `/home/username`
    - **Command substitution**: `` `date` `` or `$(date)` → replaces with command output
    - **Globbing**: `*.txt` → expands to matching files
  - Handle quoting and escaping.

---

### 🛠️ 2. **Built-in vs External Command**
- The shell checks if the command is:
  - A **built-in** (like `cd`, `echo`, `export`) — executed directly in the shell.
  - An **alias** or function.
  - If not, it proceeds to look for an **executable file**.

---

### 🔍 3. **Search in `$PATH`**
- The shell searches the directories listed in the `$PATH` environment variable to find the executable binary.
  - E.g., if you type `ls`, it might resolve to `/bin/ls`.

---

### 🧬 4. **Fork the Process**
- The shell **forks** (creates a new process) using `fork()` system call.
- The child process is a duplicate of the shell process.

---

### 🔄 5. **Execute the Command**
- In the child process, the shell calls `exec()` to replace the child with the command binary.
- `exec()` loads the binary into memory and runs it.

---

### 📤 6. **Redirect I/O (if needed)**
- If you've used redirection (`>`, `<`, `|`, etc.), the shell sets up file descriptors before `exec()`:
  - E.g., `ls > out.txt` opens/redirects `stdout` to a file.

---

### 🧱 7. **Wait for Completion**
- The parent shell process waits for the child to complete using `wait()`, unless the command runs in the background (`&`).
- When the process exits, its **exit code** is stored in `$?`.

---

### 💬 8. **Return to Prompt**
- Shell returns to the prompt, optionally showing output or errors.

---

### TL;DR
```bash
You type → Shell parses → Finds command → Forks → Executes → Waits → Shows result
```

# Whats an inode

An **inode** (index node) in Linux is a data structure used by file systems (like ext4, xfs, etc.) to store **metadata about a file or directory** — but **not** the file’s name or its actual data.

### Key information stored in an inode:
- **File type** (e.g., regular file, directory, symlink)
- **Permissions** (read/write/execute)
- **Owner and group** (UID and GID)
- **File size**
- **Timestamps** (created, modified, accessed)
- **Link count** (how many directory entries refer to this inode)
- **Pointers to data blocks** (addresses on disk where the actual file contents are stored)

### Things *not* in the inode:
- **File name** — that's stored in the directory entry that maps a name to an inode number.

### How it works:
When you list files (e.g., with `ls -i`), you can see their inode numbers. Behind the scenes:
1. The file system looks up the directory name to get the inode number.
2. It then uses that inode to fetch the file's metadata and data.

### Why inodes matter:
- A file with **zero links** (link count = 0) is deleted.
- Filesystems have a **fixed number of inodes**; you can run out of inodes before you run out of disk space.
- Understanding inodes is important for **low-level debugging**, **performance tuning**, or working on filesystems or backup tools.

# soft/hard link

In Linux, both **soft links** (also called **symbolic links**) and **hard links** are ways to reference files, but they work differently under the hood. Here's a breakdown:

---

### 🧷 Soft Link (Symbolic Link)

- **What it is**: A special file that *points* to another file or directory by name.
- **Command**: `ln -s target link_name`
- **Can link across filesystems**: ✅ Yes
- **Can link to directories**: ✅ Yes (commonly used for this)
- **If the target is deleted**: ❌ The link is broken ("dangling symlink")
- **Metadata**: Separate inode; doesn’t share file metadata (e.g., permissions, timestamps)

**Example**:
```bash
ln -s /path/to/original /path/to/link
```

---

### 🔗 Hard Link

- **What it is**: A direct reference to the same inode (same actual file data).
- **Command**: `ln target link_name`
- **Can link across filesystems**: ❌ No
- **Can link to directories**: ❌ Not by default (to avoid circular structures)
- **If the target is deleted**: ✅ The link still works, because the inode still exists
- **Metadata**: Shares inode, so it's indistinguishable from the original

**Example**:
```bash
ln /path/to/original /path/to/hardlink
```

---

### 🧠 Summary Table

| Feature                 | Soft Link (Symbolic) | Hard Link           |
|------------------------|----------------------|---------------------|
| Points to              | File name/path       | File inode          |
| Cross-filesystem       | ✅ Yes               | ❌ No               |
| Links to directories   | ✅ Yes               | ❌ No (normally)    |
| Broken if target gone  | ✅ Yes               | ❌ No               |
| Shares inode           | ❌ No                | ✅ Yes              |
| Command                | `ln -s`              | `ln`                |

---

Let me know if you want examples or want to dive deeper into how inodes work!

# Linux commands


- ip link
  - display network interfaces
- ip addr
- ping

# troubleshooting

- `uptime`
  - load average, the three numbers are exponentially damped moving sum averages with a 1 minute, 5 minute, and 15 minute constant.
- `uname -a`
  - print detailed system information
- `dmesg | tail`
  - This views the last 10 system messages
- `vmstat 1`
  - virtual memory stat
  - r: Number of processes running on CPU and waiting for a turn. This provides a better signal than load averages for determining CPU saturation, as it does not include I/O. To interpret: an “r” value greater than the CPU count is saturation.
  - free: Free memory in kilobytes. If there are too many digits to count, you have enough free memory. The “free -m” command, included as command 7, better explains the state of free memory.
  - si, so: Swap-ins and swap-outs. If these are non-zero, you’re out of memory.
  - us, sy, id, wa, st: These are breakdowns of CPU time, on average across all CPUs. They are user time, system time (kernel), idle, wait I/O, and stolen time (by other guests, or with Xen, the guest’s own isolated driver domain).
- `mpstat -P ALL 1`
  - prints CPU time breakdowns per CPU
- `pidstat 1`
  - like top prints a rolling summary instead of clearing the screen
- `iostat -xz 1`


disk is full

```bash
df -h
du -sh *

#truncate file
>filename.log
```

find and grep

```bash
find / -name httpd.conf

#directory 
find /home -type d -name directoryname
```

compnare 2 files

```bash
diff filename0 filename1

#side by side
sdiff filename0 filename1
```

cpu usage

```bash
htop


#memory
free -m
sar 1 3

vmstat 1
#r values
# memort, swap memory
# 

#cpu info
cat /proc/cpuinfo
```

network

```bash
ifconfig
ip a

sudo dmesg
sudo dmidecode

tcpdump -i any -s0

ping 

telnet
```

ssh

```bash
sestatus
iptables
tail /var/log/audit/audit.log
ssh -vvv
```

kill process

```bash
#list threads
ps -ef | grep

kill -9 id
```

```bash
perf
```