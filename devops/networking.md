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

# ICMP

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

---
