# 🌐 TFTP Client–Server File Transfer System

![Language](https://img.shields.io/badge/Language-C-blue)
![Protocol](https://img.shields.io/badge/Protocol-UDP-green)
![Networking](https://img.shields.io/badge/Networking-Socket%20Programming-orange)
![Compiler](https://img.shields.io/badge/Compiler-GCC-lightgrey)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey)

---

# 📘 Overview

This project implements a **Trivial File Transfer Protocol (TFTP)** based **Client–Server File Transfer System** using **UDP sockets in C**.

The system allows a **client to upload and download files from a server** using the standard **TFTP packet exchange mechanism**.

The implementation includes core **TFTP protocol operations**:

- **RRQ (Read Request)** – Download file from server
- **WRQ (Write Request)** – Upload file to server
- **DATA Packets** – Transfer file blocks
- **ACK Packets** – Confirm successful data receipt
- **ERROR Packets** – Report transfer failures

The project demonstrates **network programming, socket communication, and protocol implementation in C**.

---

# 🎯 Objectives

- Implement the **TFTP protocol using UDP**
- Build a **client–server architecture**
- Transfer files using **block-based packets**
- Implement **acknowledgement-based reliability**
- Support **multiple transfer modes**

---

# 🏗 System Architecture

```
                +-------------------+
                |     TFTP Client   |
                |-------------------|
                | Command Interface |
                | Request Generator |
                | File Sender       |
                | File Receiver     |
                +---------+---------+
                          |
                          | UDP Socket Communication
                          |
                +---------v---------+
                |     TFTP Server   |
                |-------------------|
                | Request Handler   |
                | File Sender       |
                | File Receiver     |
                | Error Handling    |
                +-------------------+
```

The **client sends requests**, and the **server processes them and transfers files accordingly**.

---

# 📦 TFTP Packet Structure

TFTP uses simple packet formats.

---

## 1️⃣ Request Packet (RRQ / WRQ)

```
 ----------------------------------------------------
| Opcode (2 bytes) | Filename | 0 | Mode | 0 |
 ----------------------------------------------------
```

Example:

```
Opcode = WRQ
Filename = new.txt
Mode = netascii
```

---

## 2️⃣ DATA Packet

```
 -----------------------------------------------------
| Opcode (2 bytes) | Block Number (2 bytes) | Data |
 -----------------------------------------------------
```

Example:

```
Opcode = DATA
Block Number = 1
Data = 512 bytes
```

---

## 3️⃣ ACK Packet

```
 --------------------------------------
| Opcode (2 bytes) | Block Number |
 --------------------------------------
```

Example:

```
Opcode = ACK
Block Number = 1
```

---

## 4️⃣ ERROR Packet

```
 --------------------------------------------------------
| Opcode (2 bytes) | Error Code (2 bytes) | Error Msg |
 --------------------------------------------------------
```

Example:

```
ERROR: File not found
```

---

# 🔄 TFTP Communication Sequence

## 📤 File Upload (PUT)

Client uploads a file to the server.

```
Client                           Server
------                           ------

WRQ (filename) --------------->

                 <------------- ACK(0)

DATA (block 1) --------------->

                 <------------- ACK(1)

DATA (block 2) --------------->

                 <------------- ACK(2)

DATA (block n) --------------->

                 <------------- ACK(n)
```

---

## 📥 File Download (GET)

Client downloads a file from the server.

```
Client                           Server
------                           ------

RRQ (filename) --------------->

                 <------------- DATA(1)

ACK(1) ----------------------->

                 <------------- DATA(2)

ACK(2) ----------------------->

                 <------------- DATA(n)
```

---

# 🧩 Features

### 📁 File Upload

Client uploads files using:

```
put <filename>
```

Example:

```
tftp> put new.txt
```

---

### 📥 File Download

Client downloads files using:

```
get <filename>
```

Example:

```
tftp> get test.txt
```

---

### ⚙ Transfer Modes

The program supports three transfer modes.

| Mode | Description |
|-----|-------------|
| default | Standard block transfer |
| octet | Byte-by-byte transfer |
| netascii | ASCII text transfer |

Example:

```
tftp> mode netascii
```

---

### 💬 Interactive Command Interface

Supported commands:

```
connect <server-ip>
put <filename>
get <filename>
mode <transfer-mode>
help
quit
```

Example session:

```
tftp> connect 127.0.0.1
INFO: Connected to server

tftp> put new.txt
File transfer completed successfully
```

---

# 📁 Project Structure

```
TFTP/
│
├── TFTP_Client/
│   ├── tftp_client.c
│   ├── tftp_client.h
│   ├── tftp.c
│   ├── tftp.h
│
├── TFTP_Server/
│   ├── tftp_server.c
│   ├── tftp.c
│   ├── tftp.h
│
└── README.md
```

---

# ⚙️ Program Workflow

### Client

```
main()
 │
 ├── process_command()
 │
 ├── connect_to_server()
 │
 ├── send_request()
 │
 └── receive_request()
        │
        ├── send_file()
        └── receive_file()
```

---

### Server

```
main()
 │
 ├── socket()
 ├── bind()
 │
 └── recvfrom()
        │
        └── handle_client()
                │
                ├── WRQ → receive_file()
                └── RRQ → send_file()
```

---

# 🧠 Core Concepts Implemented

This project demonstrates:

- UDP Socket Programming
- Client–Server Networking
- Protocol Implementation
- File Handling in C
- Packet-based Communication
- Error Handling
- Block-based File Transfer

---

# 🧰 Technology Stack

| Component | Description |
|----------|-------------|
| Language | C |
| Protocol | UDP |
| Compiler | GCC |
| Platform | Linux |
| Networking | Socket Programming |

---

# 🚀 How to Compile and Run

## Step 1: Start Server

```
cd TFTP_Server
gcc *
./a.out
```

Output:

```
TFTP Server listening on port 6969...
```

---

## Step 2: Run Client

Open another terminal.

```
cd TFTP_Client
gcc *
./a.out
```

---

## Step 3: Connect to Server

```
tftp> connect 127.0.0.1
```

---

## Step 4: Upload File

```
tftp> put new.txt
```

---

## Step 5: Download File

```
tftp> get new.txt
```

---

# 📊 Learning Outcomes

Through this project, I learned:

- UDP socket programming in C
- Network protocol design
- Client–server architecture
- File transfer mechanisms
- Reliable data transfer using acknowledgements

---

# 🔮 Future Enhancements

Possible improvements:

- Timeout and retransmission mechanism
- Multi-client support
- Secure file transfer
- GUI client interface
- Logging system

---

# 👨‍💻 Author

**Omkar Ashok Sawant**  
Electronics and Telecommunication Engineer  
Embedded Systems and Software Developer  

---

# 📜 License

This project is open-source and intended for **educational purposes**.