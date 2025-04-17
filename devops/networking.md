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

### Interview Highlight:
> "I’ve worked with HTTP/1.1 and HTTP/2 in production, tuning keep-alive timeouts and debugging slow endpoints with `curl -v`, HAR files, and latency breakdowns."

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

Would you like this added to the reference sheet too?