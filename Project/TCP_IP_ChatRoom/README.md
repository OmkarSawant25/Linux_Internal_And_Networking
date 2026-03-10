# 🌐 TCP/IP Multi-Client Chat Room System

![Language](https://img.shields.io/badge/Language-C-blue)
![Protocol](https://img.shields.io/badge/Protocol-TCP-green)
![Networking](https://img.shields.io/badge/Networking-Socket%20Programming-orange)
![Compiler](https://img.shields.io/badge/Compiler-GCC-lightgrey)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey)

------------------------------------------------------------------------

# 📘 Overview

This project implements a **Multi-Client Chat Room System** using **TCP
socket programming in C**.

The system allows **multiple clients to connect to a server and
communicate in real time** through **private messages or group chat**.

The server manages:

-   User authentication
-   Online user tracking
-   Message routing
-   Join/Leave notifications

This project demonstrates **client-server architecture, socket
programming, multithreading, and real-time communication using TCP**.

------------------------------------------------------------------------

# 🎯 Objectives

-   Implement TCP client--server communication
-   Support multiple clients simultaneously
-   Implement user login and signup
-   Allow private and group chat
-   Maintain online user list

------------------------------------------------------------------------

# 🏗 System Architecture

    +----------------------+
    |       Client         |
    |----------------------|
    | Login / Signup       |
    | Private Chat         |
    | Group Chat           |
    | Message Receiver     |
    +----------+-----------+
               |
               | TCP Socket Communication
               |
    +----------v-----------+
    |       Server         |
    |----------------------|
    | User Authentication  |
    | Online User List     |
    | Message Routing      |
    | Thread Management    |
    +----------------------+

The **server acts as the central communication hub**, forwarding
messages between connected clients.

------------------------------------------------------------------------

# 📦 Data Structures

## User Information Structure

``` c
struct userInfo
{
    char username[30];
    char password[20];
    char target[30];
    char message[256];
    int option;
};
```

## Online User Linked List

``` c
struct online_user
{
    char username[30];
    int sockfd;
    struct online_user *next;
};
```

------------------------------------------------------------------------

# 🔄 Chat Communication Flow

## Private Chat

Client A → Server → Client B

The server receives the message and forwards it to the target user.

## Group Chat

Client A → Server → All connected clients

The server broadcasts the message to all users except the sender.

------------------------------------------------------------------------

# 🧩 Features

### User Authentication

Supports login and signup system.

### Private Chat

Send messages directly to another user.

### Group Chat

Broadcast messages to all online users.

### Online User Notifications

Users are notified when someone joins or leaves the chat.

### Online User List

Displays currently connected users after login.

------------------------------------------------------------------------

# 📁 Project Structure

    TCPIP_CHATROOM/
    │
    ├── client/
    │   ├── client.c
    │   ├── main.h
    │
    ├── server/
    │   ├── server.c
    │   ├── main.h
    │
    ├── user.txt
    │
    └── README.md

------------------------------------------------------------------------
# ⚙️ Program Workflow

## Client–Server Communication

Client                           Server
------                           ------

Start Client  ----------------->

                 <------------- Server Waiting

Login / Signup  --------------->

                 <------------- Authentication Response

Request Online Users --------->

                 <------------- Online User List

Private Message -------------->

                 <------------- Forward to Target User

Group Message  --------------->

                 <------------- Broadcast to All Clients

Logout        ---------------->

                 <------------- Notify Users (User Offline)

---

------------------------------------------------------------------------

# 🧠 Core Concepts Implemented

-   TCP Socket Programming
-   Client-Server Architecture
-   Multithreading using pthread
-   Linked List Data Structure
-   File Handling in C
-   Real-time Chat Communication
-   Mutex Synchronization

------------------------------------------------------------------------

# 🧰 Technology Stack

  Component    Description
  ------------ --------------------
  Language     C
  Protocol     TCP
  Compiler     GCC
  Platform     Linux
  Networking   Socket Programming

------------------------------------------------------------------------

# 🚀 How to Compile and Run

## Start Server

    cd server
    gcc server.c -lpthread
    ./a.out

## Run Client

    cd client
    gcc client.c -lpthread
    ./a.out

------------------------------------------------------------------------

---

# 🎯 Learning Outcomes

- Strong understanding of TCP socket programming in Linux  
- Practical implementation of client–server architecture  
- Experience with multi-client communication using threads (pthreads)  
- Real-time message routing between connected clients  
- Managing shared resources using mutex synchronization  
- Implementation of linked list data structure for online user management  
- File handling for storing user credentials  
- Building modular and maintainable C networking programs  

---

# 👨‍💻 Author

**Omkar Ashok Sawant**\
Electronics and Telecommunication Engineering

------------------------------------------------------------------------

# 📜 License

This project is open-source and intended for **educational purposes**.
