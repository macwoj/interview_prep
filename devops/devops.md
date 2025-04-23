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
  - `vmstat 1`, r values ok, memory swap is 0, cpu 
  - `mpstat -P ALL 1`, sinlge hot cpu can be issue
  - `iostat -x 1` - disk %util, %util > 60% is an issue
  - `sar -n DEV 1` - netowrk IO
- app taking forever
  - `vmstat 1` - no idle time, user/system
  - `mpstat -P ALL 1`
  - `pidstat 1` - our app using high cpu  
  - `iostat -xz 1` - no high io
  - `strace -tp (pgrep process_name)`
- cpu consumption
  - `htop` - %CPU no idle, no app consuming
  - `mpstat 1`, CPU busy 
  - `perf record -F 99 -a -g -- sleep 10`- short liven proc
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

## network

- `ifconfig`
  - View network interface configurations
- `ip addr show`
  - newwer ifconfig to see ip and interface info
- `netstat -tulpn`
  - network stats
- `lsof -i`
- `tcpdump -i any -s0`
- `ping ip`
- `telnet`
- `traceroute`
- `ssh`
- `ss -mop`
  - socket stats

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