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

# Linux commands

- uname -a
  - print detailed system information
- ip link
  - display network interfaces
- ip addr
- ping
