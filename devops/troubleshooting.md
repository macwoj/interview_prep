# Linux commands

- `uptime`
  - load average, the three numbers are exponentially damped moving sum averages with a 1 minute, 5 minute, and 15 minute constant.
- `uname -a`
  - print detailed system information
- `dmesg | tail`
  - This views the last 10 system messages
  - kernel ring buffer messages
- `vmstat 1`
  - virtual memory stat
  - `procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----`
  - ` r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st`
  - r: Number of processes running on CPU and waiting for a turn. This provides a better signal than load averages for determining CPU saturation, as it does not include I/O. To interpret: an “r” value greater than the CPU count is saturation.
  - free: Free memory in kilobytes. If there are too many digits to count, you have enough free memory. The “free -m” command, included as command 7, better explains the state of free memory.
  - si, so: Swap-ins and swap-outs. If these are non-zero, you’re out of memory.
  - us, sy, id, wa, st: These are breakdowns of CPU time, on average across all CPUs. They are user time, system time (kernel), idle, wait I/O, and stolen time (by other guests, or with Xen, the guest’s own isolated driver domain).
- `mpstat -P ALL 1`
  - prints CPU time breakdowns per CPU
- `pidstat 1`
  - like top prints a rolling summary instead of clearing the screen
- `iostat -xz 1`
  - block device activity
  - `%user`: Time CPU spends on user-space processes (non-kernel code like apps, databases).
  - `%system`: Time spent on kernel-space tasks (e.g., I/O handling, system calls).
  - `%iowait`: Time the CPU is idle **waiting for disk I/O** to complete.
  - `%idle`: Time CPU is idle and **not waiting on I/O** (truly free time).
  - **High `%iowait`** → Disk is slow or overloaded.  
  - **High `%system`** → Many kernel-level operations (e.g., network, I/O, context switching).
- `free -m`
  - free memory stats
- `sar -n DEV 1`
  - system activity reporter
  - network interface throughput
  - `sar -n TCP,ETCP 1`
    - tcp stats
- `strace -tp (pgrep <process_name>)`
  - trace systems calls for program
  - -t timestamp, -p attach to specific pid, pgrep <process_name> - get pid
- `perf record -F 99 -a -g -- sleep 10`
- `htop` `top` `ps aux`
  - view process info
- `df -ah`
  - view filesystem disk usage, -a all, -h human
- `du -sh * | sort -h`
  - disk usage 
- `lsof`
  - list open file descriptors
  - `lsof -i`
    -  all network connections (TCP/UDP) and listening sockets.
  - `lsof -i :<port>` `lsof -i tcp` `lsof -i udp`

## perf

- Hardware Counters
  - `cpu-cycles`: Total CPU cycles  
  - `instructions`: Executed instructions  
  - `cache-references`: Cache accesses  
  - `cache-misses`: Cache misses  
  - `branches`: Branch instructions  
  - `branch-misses`: Branch mispredictions  
  - `bus-cycles`: Memory bus cycles  
  - `ref-cycles`: Constant-rate reference cycles  
- Software Counters
  - `page-faults`: Total page faults  
  - `minor-faults`: No I/O needed  
  - `major-faults`: With disk I/O  
  - `context-switches`: Voluntary/involuntary switches  
  - `cpu-migrations`: Task moved between CPUs  
  - `task-clock`: Time task ran on CPU  
  - `cpu-clock`: Total CPU time used  
  - `alignment-faults`: Misaligned accesses  
  - `emulation-faults`: Instructions emulated by kernel  

```bash
perf stat -e cycles,instructions,cache-misses ./my_app
```

## troubleshooting

- app latency
  - `top` - check load, CPU is idle, IO bound
  - `vmstat 1`, r values in small range (r values >cpu count means saturation), memory swap is 0, cpu-wa (wait io) is constant -> means disc bottleneck
  - `mpstat -P ALL 1`, hot cpu can be issue, look at CPU column and imbalance
  - `iostat -x 1` - disk %util is high, %util > 60% is an issue
  - `sar -n DEV 1` - network IO, idle
- app taking forever
  - `vmstat 1` - CPU no idle time (id), high user/system (us/sy)
  - `mpstat -P ALL 1` - %usr and %sys matches vmstat output
  - `pidstat 1` - our app using high cpu  (%usr/%system)
  - `iostat -xz 1` - no high io, await/%util are low
  - `sar -n DEV 1` - network idle
  - `strace -tp (pgrep process_name)`
- cpu consumption
  - `htop` - %Cpu(s) is high at the top, user time (us) is high, %CPU column doesn't have any high cpu tasks?
  - `mpstat 1`, CPU busy %usr/%sys high
  - `iostat -x 1` - %util is 0
  - `sar -n DEV 1` - network idle
  - `perf record -F 99 -a -g -- sleep 10`- short lived proc
- check open ports
  - `netstat -tulpn`
- mount
  - `ls /mnt`
  - `mount /dev/sda2 /mnt`
  - `mount` - list all mounts
  - `cat /etc/fstab` - mounts on start
  - view vnfs
    - `mount -t nfs,nfs4`
- Finding io bottleneck
  - **Check `iostat -xz`**:
    - **High `%iowait`?** → Disk is likely the bottleneck.
    - **Low `%idle` + high `%system`?** → System under CPU pressure or too many I/O interrupts.
    - `r/s`, `w/s`: read/write operations per second.
    - `await`: average time per I/O request → high values = slow disk.
    - `util`: % of time disk was busy → >80% = saturation.
    - `svctm`: average service time per I/O → high = disk strain.


### **Scenario**: System is reported to be slow, especially during peak traffic.

**Step-by-step `vmstat` usage**:

```sh
vmstat 1 10
```

This gives 10 samples at 1-second intervals.

**Sample output**:
```
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 2  1  10240  12000  10000 300000    5    6   100   200  150  200 10  5 70 15  0
 3  2  10240  10000   8000 280000    0    0   500   300  200  300 12  7 60 21  0
...
```

**What to look for**:

- `r > 1`: more runnable processes than CPU cores → CPU contention
- `b > 0`: processes blocked on I/O → possible disk bottleneck
- `si`, `so`: non-zero swap in/out → system is memory constrained
- `wa`: high `%wa` (e.g. >10%) → waiting on I/O
- `us`, `sy`: shows how CPU time is spent (user vs system)

**Interpretation**:

- High `wa` (15–21%) and non-zero `b` suggests **disk I/O is the bottleneck**
- Swap usage is small but non-zero (`si` and `so` > 0), indicating **memory pressure**
- CPU is not saturated (`id` is 60–70%), so not the main issue

**Next steps**:

- Use `iostat` or `iotop` to confirm disk I/O bottlenecks
- Check which processes are causing I/O using `iotop`
- Consider adding RAM if swap is frequently used
- Tune disk I/O or move I/O-heavy apps to faster storage

---

### **Scenario**: A web server is experiencing slow response times. You suspect disk I/O might be the bottleneck.

**Step-by-step with `iostat`**:

1. **Run iostat with useful flags**  
   ```bash
   iostat -xz 1
   ```
   - `-x`: extended stats (gives r/s, w/s, %util, await, etc.)
   - `-z`: omits devices with no activity
   - `1`: updates every second

2. **Look for high %util**
   - `%util` near 100% means the disk is fully busy.
   - Example:
     ```
     Device:  %util
     sda      98.53
     ```
     This shows that `/dev/sda` is nearly maxed out.

3. **Check `await` and `svctm`**
   - `await`: total time I/O requests wait, includes queue time
   - `svctm`: service time (just time taken to handle the request)
   - If `await` >> `svctm`, the disk is likely overloaded or there's a queue build-up.

4. **Check `r/s`, `w/s` (reads/writes per second)**
   - Helps identify if reads or writes dominate.  
   - Useful if tuning caching or I/O scheduling.

5. **Check if it’s a specific mount or device**
   - Combine `iostat` output with `mount` and `df` to trace which filesystem is on the affected device.

**Follow-up actions**:
- Add caching (e.g. page cache, Redis) if reads dominate
- Tune `vm.dirty_ratio` if heavy writeback causes lag
- Move I/O heavy workloads to another disk
- Check for filesystem-level issues or fragmentation
- Use faster disk (e.g. SSD) or RAID

---

### 🔧 **System Troubleshooting**

#### 1. **Q: A Linux server is slow. What’s your first step?**
**A:**
- Run `top`, `htop`, or `uptime` to check:
  - Load average vs. number of CPU cores
  - CPU or memory bottlenecks
- Use `vmstat 1`, `iostat -xz 1`, or `dstat` to assess:
  - CPU steal/wait time
  - Disk I/O issues
  - Swap usage

---

#### 2. **Q: How do you check if a process is in a zombie state?**
**A:**
- Use `ps aux | grep 'Z'` or `ps -ef -o pid,ppid,state,cmd | grep Z`
- A zombie (`Z`) process has completed but is not cleaned up by its parent.

---

#### 3. **Q: How do you find which process is using the most memory?**
**A:**
- `ps aux --sort=-%mem | head`
- Or use `smem` or `top` sorted by memory.

---

### 🧠 **Resource Usage & Bottlenecks**

#### 4. **Q: How do you identify high disk I/O?**
**A:**
- `iostat -xz 1`: Look at `await`, `util`, and `avgqu-sz`
- `iotop` (if enabled): Shows real-time disk I/O by process

---

#### 5. **Q: How do you check memory usage and diagnose swap issues?**
**A:**
- `free -m` to check used/free/swap memory
- If swap is heavily used:
  - Use `vmstat 1` to check `si`/`so` columns (swap in/out)
  - Use `top` or `smem` to find memory-heavy processes

---

### 🌐 **Networking Issues**

#### 6. **Q: How do you troubleshoot network connectivity issues?**
**A:**
- `ping`, `traceroute`, or `mtr` to test connectivity
- `netstat -tulnp` or `ss -tulnp` to see open ports
- `tcpdump` or `wireshark` to capture traffic
- `iptables -L` or `nft list ruleset` for firewall rules

---

#### 7. **Q: A port is not reachable — what do you check?**
**A:**
1. Is the service listening? → `ss -tuln | grep <port>`
2. Firewall rules blocking it? → `iptables`, `nftables`, or `firewalld`
3. SELinux or AppArmor restrictions? → `getenforce`, `audit2why`
4. Is the server itself reachable? → `ping`, `telnet`, `nc`

---

### 🧰 **Filesystem & Logs**

#### 8. **Q: How do you find which directory is using the most space?**
**A:**
- `du -sh *` in `/`, `/var`, `/home`, etc.
- `du -ahx / | sort -rh | head -20`

---

#### 9. **Q: How do you monitor log files for errors in real-time?**
**A:**
- `tail -f /var/log/syslog` or `/var/log/messages`
- `journalctl -f` (for systemd systems)
- Use `grep -i error` or `less +F` for highlighting

---

#### 10. **Q: A cron job isn't running. How do you debug it?**
**A:**
- `crontab -l` to confirm it’s scheduled
- Check `/var/log/cron`, `syslog`, or `journalctl`
- Ensure script is executable and has correct shebang (`#!/bin/bash`)
- Check environment differences (cron has limited `PATH`)

---

Great — here’s a **focused set of Linux troubleshooting interview Q&A for SRE roles**, emphasizing **system reliability**, **performance**, and **deep diagnostics**. These are tailored to senior IC or SRE roles at companies like Google, Meta, or Datadog.

---

#### 1. **Q: A production host is reporting high load average. What’s your process to investigate?**
**A:**
- Check with `uptime` or `top`:
  - Compare load average vs. CPU cores
- Use:
  - `top`/`htop` → Check for CPU/IO wait
  - `vmstat 1` → See if waiting on IO (`wa`) or blocked processes (`b`)
  - `iostat -xz 1` → Look for high disk wait times
  - `dstat` or `pidstat` → Per-process stats
- Rule out runaway processes, stuck threads, or disk bottlenecks.

---

#### 2. **Q: How would you debug a container using high CPU on a Kubernetes node?**
**A:**
- Use `kubectl top pod`, then drill into the node:
  - `docker stats` or `crictl top`
  - `ps aux --sort=-%cpu`, `perf top` for flame graph-level insights
- Use `strace -p <pid>` or `perf record` to analyze syscall behavior
- Look for tight loops, lock contention, or JIT hot paths.

---

#### 3. **Q: A service in Kubernetes is timing out. Where do you start?**
**A:**
1. **Check service health**:
   - `kubectl describe pod`, `logs`, and `events`
2. **Validate network connectivity**:
   - `nslookup`, `curl`, `kubectl exec <pod> -- curl <endpoint>`
3. **Inspect kube-proxy / iptables / CNI layer**:
   - `iptables -L -t nat`, `conntrack -L`
4. **Trace packets**:
   - Use `tcpdump`, `traceroute`, `mtr` from pod/node

---

#### 4. **Q: How do you investigate a process stuck in uninterruptible sleep (`D` state)?**
**A:**
- Check `ps -eo pid,stat,comm,wchan | grep ^D`
- Usually IO-related (waiting on disk/NFS/socket)
- Use:
  - `lsof -p <pid>` to see file handles
  - `strace -p <pid>` (may hang if it’s truly stuck)
  - Check dmesg or `iotop` for storage issues
  - Possible causes: disk failure, NFS hang, kernel bugs

---

#### 5. **Q: A host is repeatedly going OOM (Out of Memory). What do you do?**
**A:**
- `dmesg | grep -i oom` → confirm kernel OOM killer activity
- `journalctl -k` or `/var/log/messages`
- Use `smem`, `ps aux --sort=-%mem`, or `cgroups` stats to find leaky process
- Validate swap behavior with `vmstat`, `free -m`
- Implement cgroup memory limits or container memory caps

---

#### 6. **Q: How do you identify and resolve file descriptor leaks?**
**A:**
- `lsof -p <pid>` → list open fds
- `ls /proc/<pid>/fd/ | wc -l` → count
- `ulimit -n` → soft/hard limits
- Alerting via Prometheus node exporter (`node_filefd_allocated`)
- Fix via app-level fd cleanup, increase limits, or restart loop leaks

---

#### 7. **Q: A node isn’t responding to SSH. How do you debug remotely?**
**A:**
- Use out-of-band access (e.g., AWS SSM, IPMI, or GCP Serial Console)
- Check `journalctl` or `/var/log/auth.log` for `sshd` failures
- Validate that `sshd` is running and port 22 is open
- Check `iptables`, network ACLs, or `fail2ban` blocks
- If it’s alive but unresponsive, may need to trigger soft reboot via cloud API

---

#### 8. **Q: What tools do you use to trace system calls or performance issues?**
**A:**
- **strace** – syscall tracing
- **lsof** – file and socket usage
- **perf** – CPU profiling
- **bcc-tools / bpftrace** – in-kernel tracing (e.g., `execsnoop`, `opensnoop`)
- **gdb** – for native binary inspection
- **systemtap / eBPF** – deep tracing of kernel behavior

---

#### 9. **Q: How do you ensure kernel logs are monitored and acted upon?**
**A:**
- Use `journalctl -k` or `dmesg` for access
- Centralize via log aggregation (e.g., Fluentd, rsyslog, journald → Elasticsearch)
- Alert on patterns: OOM kills, `ext4` errors, hardware failures
- Integrate with incident tooling (e.g., PagerDuty, Grafana alerts)

---

#### 10. **Q: What would you do if you find high CPU in `ksoftirqd`?**
**A:**
- Indicates excessive soft IRQ handling (typically due to high network/disk IO)
- Check with:
  - `top` or `pidstat -w`
  - `mpstat -P ALL`, `cat /proc/interrupts`
- Possible mitigations:
  - Tune NIC IRQ affinity (`irqbalance`, `ethtool`)
  - Use multi-queue NICs with RSS
  - Offload packet processing (GRO/LRO, XDP)

---

disk is full

```bash
df -h
du -sh *

#see how many inodes are used
df -i

#truncate file
>filename.log
```

find and grep

```bash
find / -name httpd.conf

#directory 
find /home -type d -name directoryname
find /path/to/directory -type f -mmin -30
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


#cpu info
cat /proc/cpuinfo
```



```bash
sestatus
iptables
tail /var/log/audit/audit.log
ssh -vvv
```

```bash
perf
```