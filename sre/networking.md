## 🔹 **TCP (Transmission Control Protocol)**

**Purpose:** Reliable, ordered, and error-checked delivery of data between applications over IP.

### Key Concepts:
- **Connection-oriented:** Uses a **3-way handshake** (SYN, SYN-ACK, ACK).
- **Reliable Delivery:** 
  - **Sequence numbers** ensure data is reassembled in order.
  - **Acknowledgments (ACKs)** confirm receipt.
  - **Retransmissions** on packet loss (via timeout or duplicate ACKs).
- **Flow Control:** Prevents overwhelming the receiver (via **TCP window size**).
- **Congestion Control:** Algorithms like **TCP Reno**, **Cubic** detect and avoid network congestion.
- **Streaming Protocol:** No message boundaries—application defines them.

### Interview Highlight:
> "TCP ensures reliable delivery over an unreliable IP layer using mechanisms like retransmissions and congestion control. I’ve debugged packet loss by analyzing TCP retransmits in `tcpdump` and tuned window sizes for high-latency links."

---

## UDP

UDP (User Datagram Protocol) is a connectionless, lightweight transport layer protocol in the IP suite. It sends datagrams without establishing a connection, meaning there's no guarantee of delivery, ordering, or error correction. It's useful for real-time applications like DNS, VoIP, and video streaming where speed is more critical than reliability.

## ICMP

ICMP (Internet Control Message Protocol) operates at the network layer and is used for sending control messages and error reports—e.g., when a service is unreachable or a packet’s TTL has expired. ICMP is not used to send application data. Tools like ping and traceroute rely on ICMP.

## 🔹 **TLS (Transport Layer Security)**

**Purpose:** Adds **confidentiality**, **integrity**, and **authentication** on top of TCP.

### Key Concepts:
- **Handshake Phase:**
  - Negotiates **cipher suite**, **protocol version**.
  - Performs **authentication** (usually server via certificate).
  - Establishes **session keys** using asymmetric encryption (e.g., ECDHE).
- **Record Protocol:** Encrypts data with symmetric encryption (AES, ChaCha20).
- **Perfect Forward Secrecy:** Achieved via ephemeral key exchanges like ECDHE.
- **TLS Termination:** Offloaded to load balancers in scalable setups.

### Interview Highlight:
> "TLS protects data in transit. I’ve configured mutual TLS between services and used tools like `openssl s_client` to debug cert issues and handshake failures."

## Mutual TLS

Mutual TLS (mTLS) is an extension of the standard TLS (Transport Layer Security) protocol that provides **two-way authentication** between a client and a server.

### What Happens in Regular TLS
- **Server Authentication**: The server presents its certificate to prove its identity to the client.
- **Client Verification**: The client checks the server's certificate against trusted CAs (Certificate Authorities).

### What Happens in Mutual TLS
- **Client Authentication** is added:
  - Server still presents its certificate.
  - **Client also presents a certificate** to the server.
  - The server validates the client's certificate (against its own list of trusted CAs).
  
### Key Features
- **Two-way trust**: Both sides verify each other’s identity.
- **Certificates**: X.509 certificates are used for authentication.
- **Encrypted channel**: All communication is encrypted, just like regular TLS.

### Use Cases
- Microservices communicating securely within a network
- Zero-trust architectures
- B2B APIs where client identity must be verified
- Securing gRPC or Kafka endpoints

### Benefits
- Strong identity verification
- Prevents impersonation and man-in-the-middle attacks
- Fine-grained access control based on client identity

---

## 🔹 **HTTP (HyperText Transfer Protocol)**

**Purpose:** Application-layer protocol for web communication.

### HTTP/1.1:
- **Text-based**, request-response model.
- Supports persistent connections (`Connection: keep-alive`).
- Limitations: Head-of-line blocking, no multiplexing.

### HTTP/2:
- **Binary protocol**, multiplexing streams over a single TCP connection.
- **Header compression** (HPACK), server push.
- Still over TCP, so subject to TCP’s HOL blocking.

### HTTP/3:
- Runs over **QUIC** (UDP-based).
- Avoids TCP HOL blocking, faster handshakes.
- Encrypts all payloads and metadata (even headers).

## HTTPS

Sure, here’s the original explanation rewritten as a single compact list:

- HTTPS = HTTP over TLS, which provides encryption, authentication, and integrity
- A TCP 3-way handshake establishes the connection first:
  - Client sends SYN
  - Server replies with SYN-ACK
  - Client responds with ACK
- After TCP, the TLS handshake begins:
  - Client Hello: sends supported TLS version, cipher suites, random value
  - Server Hello: replies with selected cipher suite, its certificate (with public key), and its random value
  - Client verifies the server's certificate using a trusted Certificate Authority (CA)
  - Client generates a pre-master key, encrypts it with the server's public key (asymmetric encryption), and sends it to the server
  - Both sides derive the same session key using the client random, server random, and pre-master key
  - Finished messages are exchanged using the session key to confirm handshake completion
  - Encrypted communication begins using symmetric encryption (e.g. AES)
- Asymmetric encryption (RSA, ECDSA) is used only during the handshake for key exchange and authentication
- Symmetric encryption (AES, ChaCha20) is used for the actual data transfer due to its speed and efficiency

---

## 🔹 **DNS (Domain Name System)**

**Purpose:** Translates domain names to IP addresses.

### Key Concepts:
- **Recursive vs Iterative resolution:**
  - Recursive resolver handles querying root → TLD → authoritative.
- **Caching:** TTL-driven; reduces latency and load.
- **Record Types:** A/AAAA (IP), CNAME (alias), MX (mail), TXT, NS.
- **DNS over HTTPS (DoH)/TLS (DoT):** Encrypts DNS to prevent eavesdropping.
- **Failure scenarios:** Propagation delays, stale caches, missing records.

### Interview Highlight:
> "I’ve diagnosed DNS issues in microservices by checking stale records and propagation delays. Tools like `dig`, `nslookup`, and `tcpdump` helped validate query chains and TTLs."

---

## 🔹 Sample Interview Answer Prompt:

**Q:** _"Can you walk me through what happens when you open a browser and go to https://example.com?"_

**A:**
> "Sure. The browser first uses **DNS** to resolve `example.com` to an IP address. It sends a request to a **recursive resolver**, which may use cached results or query authoritative servers. Once the IP is known, a **TCP** connection is established via a 3-way handshake. Then, a **TLS handshake** secures the channel: negotiating keys, validating certificates, and establishing encryption. Over this secure connection, an **HTTP** request is sent (usually HTTP/2). The server responds with the content, which the browser renders."

## 🧭 DNS Resolution: What Happens When You Open a Website

### Step-by-Step Flow:

#### 1. **Browser Cache Check**
- The browser checks its **internal DNS cache** to see if it already knows the IP for `example.com`.
- If found and TTL hasn’t expired, no external query is made.

#### 2. **OS Resolver Cache**
- If the browser doesn't have it, the OS checks its **system-wide DNS cache** (e.g., `nscd` or `systemd-resolved` on Linux, `dnsapi` on Windows).

#### 3. **Query Sent to Recursive Resolver**
- If not cached locally, the OS sends a DNS query (UDP or TCP, port 53) to a **recursive DNS resolver** — often configured via DHCP (e.g., Google 8.8.8.8 or ISP's resolver).

#### 4. **Resolver Cache Check**
- The recursive resolver checks its own cache. If it finds the answer (within TTL), it replies immediately.

#### 5. **Root Server Lookup**
- If not cached, the resolver queries a **Root DNS server**:
  - Example: "I need to resolve `.com` TLD."
  - Root responds with the IP addresses of the **TLD name servers** for `.com`.

#### 6. **TLD Server Lookup**
- The resolver asks the `.com` TLD server:
  - “Where is the authoritative name server for `example.com`?”
  - It replies with the **NS records** for the authoritative server.

#### 7. **Authoritative Server Query**
- The resolver asks the authoritative name server:
  - “What is the A or AAAA record for `example.com`?”
  - It replies with the IP address (e.g., `93.184.216.34`).

#### 8. **Caching and Response**
- The resolver caches this result for the TTL (say 300s).
- It returns the IP to the OS, which caches it and passes it to the browser.
- The browser uses this IP to establish a **TCP connection** (and then TLS + HTTP).

---

## 🛠️ DNS Tools to Mention

- `dig example.com` – for manual DNS lookups.
- `dig +trace example.com` – trace the entire resolution path.
- `nslookup example.com` – interactive resolver queries.
- `tcpdump port 53` – observe DNS queries on the network.
- `systemd-resolve --status` – view cache and configuration on Linux.

---

## 🔐 DNS Variants & Security

- **DNS over TLS (DoT)** – TCP port 853, encrypts between client and resolver.
- **DNS over HTTPS (DoH)** – over HTTPS (port 443), widely used by browsers (e.g., Firefox, Chrome).
- **DNSSEC** – provides origin authentication via digital signatures but needs to be validated by the resolver.

## DNS Stored in:

- `/etc/resolv.conf` – primary DNS resolver configuration (nameservers, search domains)  
- `/etc/hosts` – static hostname-to-IP mappings, checked before DNS  
- `/etc/nsswitch.conf` – defines lookup order, e.g., `hosts: files dns`  
- `/run/systemd/resolve/resolv.conf` – used by `systemd-resolved` for DNS settings  
- `resolvectl status` – shows current DNS status under systemd  
- `/etc/NetworkManager` – stores DNS configs managed by NetworkManager  
- `nmcli` – CLI tool to view or set DNS via NetworkManager

---

## 🧠 Interview Soundbite

> "When resolving a domain, the OS contacts a recursive DNS resolver. If it’s not cached, the resolver walks the DNS hierarchy: root → TLD → authoritative. Once the IP is retrieved, it’s cached and used for TCP and TLS handshakes. Tools like `dig +trace` help diagnose resolution issues, and in secure environments, I’ve deployed DNS over TLS and validated DNSSEC signatures."

---

# How does packet routing work?

Here's a compact breakdown of how packet routing works:

- **Routing by IP address**  
  The source computer uses the destination IP address in the packet header to determine where to send the packet. It checks its local routing table to find the next hop.

- **Local routing table**  
  This table contains routes for:
  - Directly connected networks (via interfaces like `eth0`)
  - A default gateway (used if no specific route is found)

- **Next hop and gateways**  
  If the destination is not on the local subnet, the packet is forwarded to the configured **default gateway** (router). The router repeats the process, forwarding the packet to the next router along the path.

- **How packets move across networks**  
  - Each router along the way uses its own routing table to decide the next hop
  - Packets may take different paths to reach the same destination (IP routing is stateless)
  - Protocols like BGP (between ISPs) and OSPF (within large networks) determine routes dynamically

- **ARP for local delivery**  
  Before sending a packet on the local network, the system uses ARP (Address Resolution Protocol) to resolve the destination IP to a MAC address.

- **TTL and fragmentation**  
  Each packet has a TTL (time to live) to prevent infinite loops. Packets may be fragmented if they exceed the MTU of a network segment.


# traceroute

On Linux, `traceroute` typically uses **UDP packets** by default, unless specified otherwise. Here's how it works step by step:

- Sends UDP packets to the destination on a high-numbered port (default: 33434)
- Starts with TTL = 1
- Each router that receives the packet:
  - Decrements TTL
  - If TTL reaches 0, sends back **ICMP "Time Exceeded"**
- `traceroute` records the responding router’s IP and the round-trip time
- TTL is incremented by 1 and the process repeats
- When the packet reaches the final destination:
  - The host responds with **ICMP "Port Unreachable"** (because the port is likely closed)
- That signals the end of the route

Options on Linux:
- `-I`: use ICMP Echo Request instead of UDP (like Windows `tracert`)
- `-T`: use TCP SYN packets (useful to avoid firewall drops)

# `ifconfig`
  - View network interface configurations
  - lo loopback interface

```
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.1.100  netmask 255.255.255.0  broadcast 192.168.1.255
        inet6 fe80::1a2b:3c4d:5e6f:7g8h  prefixlen 64  scopeid 0x20<link>
        ether 00:11:22:33:44:55  txqueuelen 1000  (Ethernet)
        RX packets 12345  bytes 9876543 (9.8 MB)
        RX errors 0  dropped 2  overruns 0  frame 0
        TX packets 6789  bytes 4567890 (4.5 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

### Explanation:

- `eth0`: Name of the network interface

- `flags=4163<UP,BROADCAST,RUNNING,MULTICAST>`:
  - `UP`: Interface is enabled
  - `BROADCAST`: Supports broadcast
  - `RUNNING`: Interface is operational
  - `MULTICAST`: Can send/receive multicast traffic

- `mtu 1500`: Maximum Transmission Unit, max packet size in bytes
- `inet 192.168.1.100`: IPv4 address assigned to the interface
- `netmask 255.255.255.0`: Subnet mask
- `broadcast 192.168.1.255`: Broadcast address for the subnet
- `inet6 fe80::...`: IPv6 address with link-local scope
- `ether 00:11:22:33:44:55`: MAC (hardware) address
- `txqueuelen 1000`: Length of transmit queue
- `(Ethernet)`: Link type
- `RX packets 12345`: Total received packets
- `RX bytes 9876543`: Total bytes received (in bytes and human-readable)
- `RX errors, dropped, overruns, frame`: Receive errors and dropped packets
- `TX packets 6789`: Total transmitted packets
- `TX bytes 4567890`: Total bytes transmitted
- `TX errors, dropped, overruns, carrier, collisions`: Transmit errors and problems

# netstat

The command `netstat -tulpn` displays active network connections and listening ports on a Linux system, along with the processes that own them.

Here's what each flag means:

- `-t` : TCP connections
- `-u` : UDP connections
- `-l` : Show only listening sockets
- `-p` : Show the process ID and name of the program using the socket
- `-n` : Show numerical addresses instead of resolving hostnames and ports

### Sample Output
```
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1056/sshd
tcp6       0      0 :::80                   :::*                    LISTEN      1480/nginx
udp        0      0 0.0.0.0:68              0.0.0.0:*                           778/dhclient
```

### Columns Explained

- **Proto**: Protocol (`tcp`, `udp`, `tcp6`, or `udp6`)
- **Recv-Q**: Received queue (data received but not yet processed by the app)
- **Send-Q**: Sent queue (data sent but not yet acknowledged by the remote host)
- **Local Address**: IP address and port on the local machine
- **Foreign Address**: Remote IP address and port (will be `*` for listening sockets)
- **State**: TCP state (like `LISTEN`, `ESTABLISHED`). For UDP, this is usually blank.
- **PID/Program name**: Process ID and name of the application using the port

The `netstat -r` command displays the **kernel routing table** on a Linux system. This table determines how network packets are routed to their destination.

Here’s how to interpret the output:

```
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
0.0.0.0         192.168.1.1     0.0.0.0         UG      0   0        0    eth0
192.168.1.0     0.0.0.0         255.255.255.0   U       0   0        0    eth0
```

Key columns:

- **Destination**: The destination network or IP address.
  - `0.0.0.0` means "default route" (used when no other route matches).
- **Gateway**: The gateway IP address through which to send packets.
  - `0.0.0.0` means packets go directly to the destination (no gateway needed).
- **Genmask**: The subnet mask for the destination.
- **Flags**:
  - `U` = route is up
  - `G` = uses a gateway
- **Iface**: The network interface used (e.g., `eth0`, `wlan0`).

In summary:

- It shows **how your system decides where to send packets**.
- Use it to debug **routing issues**, especially default gateway problems or when multiple interfaces are in use.


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
- `telnet`
- `ss -mop`
  - socket stats

---

# sar

The `sar -n DEV 1` command is used to monitor network device statistics on Linux, updated every second. Here's a breakdown:

- `sar`: System Activity Reporter, a tool that collects, reports, and saves system activity information.
- `-n DEV`: Reports network device statistics (e.g., eth0, lo).
- `1`: Interval in seconds between each report (in this case, every 1 second).

### Sample Output Columns:
You’ll typically see output like this:

```
Time       IFACE   rxpck/s  txpck/s   rxkB/s   txkB/s   rxcmp/s  txcmp/s  rxmcst/s
12:00:00   eth0    102.00   120.00    12.00    15.00    0.00     0.00     1.00
```

### Key Columns:
- `IFACE`: Network interface name (e.g., eth0)
- `rxpck/s`: Received packets per second
- `txpck/s`: Transmitted packets per second
- `rxkB/s`: Received kilobytes per second
- `txkB/s`: Transmitted kilobytes per second
- `rxcmp/s`, `txcmp/s`: Compressed packets received/transmitted per second (usually 0)
- `rxmcst/s`: Multicast packets received per second

### Use Case:
Use `sar -n DEV 1` to:
- Monitor real-time network bandwidth usage
- Identify spikes in traffic
- Troubleshoot network throughput issues

---

# Ports

Top 10 most commonly used service ports:

- 22: SSH — secure remote login
- 23: Telnet — insecure remote login (legacy)
- 25: SMTP — email sending
- 53: DNS — domain name resolution
- 80: HTTP — web traffic (unencrypted)
- 110: POP3 — receive email (legacy)
- 143: IMAP — receive email (modern)
- 443: HTTPS — web traffic (secure)
- 3306: MySQL — database access
- 3389: RDP — Windows remote desktop

# Questions

- **How does packet routing work?**  
  Routing table (`ip route`) determines next hop; kernel uses longest prefix match.

- **What’s the difference between TCP and UDP?**  
  TCP: reliable, connection-based. UDP: faster, connectionless, no retransmit.

- **What is a socket?**  
  Endpoint for network communication; created via `socket()` syscall.

- **How does DNS resolution work?**  
  Resolver queries `/etc/hosts`, then nameservers from `/etc/resolv.conf`.

- **What is ARP?**  
  Resolves IP to MAC in local network using broadcast.

- **What is MTU and what happens if exceeded?**  
  Maximum Transmission Unit; exceeding causes fragmentation or packet drop.

- **How to check open ports?**  
  `ss -tuln`, `netstat -tuln`, or `lsof -i`.

- **What is NAT?**  
  Translates internal IPs to public IPs for outbound traffic (masquerading).

- **How do you capture packets?**  
  Use `tcpdump`, `wireshark`, or `ip -s link` for interface stats.

- **What is the difference between `iptables` and `nftables`?**  
  `iptables` is legacy; `nftables` is newer, with unified framework and better performance.

---

**1. DNS resolution failure**

- **symptom:** can't resolve `example.com`
- **tools:**
  - `dig example.com` – test DNS query
  - `cat /etc/resolv.conf` – check configured nameservers
  - `ping 8.8.8.8` – test raw network connectivity
  - `systemd-resolve --status` or `nmcli dev show` – debug local resolver status

---

**2. Cannot reach external host**

- **symptom:** `curl https://example.com` fails
- **tools:**
  - `ping example.com` – test connectivity
  - `traceroute example.com` – find where packets drop
  - `curl -v https://example.com` – show connection details
  - `iptables -L -v` or `nft list ruleset` – check for dropped traffic
  - `ip route` – confirm default route
  - `ip a` – ensure IP is assigned

---

**3. High network latency or poor throughput**

- **symptom:** slow download/upload
- **tools:**
  - `ping -c 10 host` – check RTT stability
  - `mtr host` – live path trace + packet loss
  - `iperf3 -s` / `iperf3 -c` – test bandwidth between two nodes
  - `ethtool eth0` – NIC duplex/speed check
  - `netstat -s` or `ss -s` – check TCP retransmits

---

**4. Port connectivity issue (e.g. web server not reachable)**

- **symptom:** can't connect to a service
- **tools:**
  - `ss -tuln` or `netstat -tuln` – check if port is open
  - `lsof -i :port` – see which process is listening
  - `iptables -L` – check firewall rules
  - `nc -zv localhost 80` – test if port is reachable locally
  - `curl -v http://localhost:80` – test HTTP connection

---

**5. ARP-related issues (e.g. can't ping local machine)**

- **symptom:** ping fails on LAN
- **tools:**
  - `ip neigh` – check ARP table
  - `arping IP` – test ARP-level reachability
  - `tcpdump -e arp` – sniff ARP packets
  - `bridge link` / `brctl showmacs` – debug MAC learning on bridges

---

**6. Interface issues or drops**

- **symptom:** flaky or dropping network
- **tools:**
  - `ip -s link` – check packet drops/errors
  - `dmesg | grep eth0` – kernel messages for NIC
  - `ethtool -S eth0` – NIC stats (e.g. crc errors, drops)
  - `nmcli` / `nmtui` – reinitialize connections

To view dropped packets in Linux, you can use these commands:
- `netstat -s`  
  Shows protocol statistics including dropped packets per protocol (deprecated on newer systems)
- `ss -s`  
  Summary of socket statistics including dropped packets
- `ip -s link`  
  Displays interface statistics including RX/TX dropped packets per network interface
- `ethtool -S <interface>`  
  Detailed per-interface stats (driver-dependent) including specific drop reasons
- `sar -n DEV 1`  
  Real-time network device stats every 1 second, including drops (requires `sysstat`)
- `dmesg | grep -i drop`  
  Kernel logs that may show drop-related events

To view TCP retransmissions in Linux:
- `ss -s`  
  Shows TCP socket statistics, including retransmitted segments.
- `netstat -s | grep retransmit`  
  Displays TCP retransmission statistics (deprecated but still works on many systems)
- `cat /proc/net/netstat | grep Tcp`  
  Look for lines like `TcpExt` with fields such as `TCPRetransSegs`
- `sar -n TCP,ETCP 1`  
  -n TCP,ETCP: Adds extended TCP stats (ETCP) including retransmissions
  `12:00:01  active/s  passive/s  iseg/s  oseg/s  retrans/s`
- `tcpdump -i <interface> tcp and 'tcp[13] & 0x04 != 0'`  
  Captures packets with the RST flag, which may suggest retransmission issues
- `perf record -e tcp:tcp_retransmit_skb -a`  
  Trace retransmit events at the kernel level (requires `perf` and kernel tracing support)