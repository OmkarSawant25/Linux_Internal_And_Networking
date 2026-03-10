/*
===============================================================================
Project Title : TCP/IP Chat Room System (Server)
Name          : Omkar Ashok Sawant
Batch Id      : 25021C_309
Date          : 07/03/2026
Language      : C Programming
Protocol      : TCP (Transmission Control Protocol)
===============================================================================

Project Overview:
    This program implements the server-side of a multi-client TCP/IP
    chat room system. The server manages multiple client connections,
    authenticates users, maintains the list of online users, and routes
    messages between clients.

    The server supports both private messaging and group messaging
    among connected users.

Supported Operations:
        - User Login Authentication
        - User Registration (Signup)
        - Maintain Online User List
        - Private Messaging Between Users
        - Group Messaging to All Users
        - User Join/Leave Notifications

Features:
    1. Multi-Client Support:
       - Handles multiple clients simultaneously using threads.

    2. User Authentication:
       - Stores user credentials in a text file.

    3. Online User Management:
       - Maintains a linked list of connected users.

    4. Join/Leave Notifications:
       - Broadcasts when a user joins or leaves the chat.

    5. Private Chat Routing:
       - Delivers messages to specific users.

    6. Group Chat Broadcasting:
       - Sends messages to all connected users.

    7. Thread Synchronization:
       - Uses mutex locks to protect shared resources.

    8. Crash Handling:
       - Notifies all users if the server crashes.

File Structure:
    - server.c  : Server implementation and client handling
    - main.h    : Shared data structures and protocol definitions
    - user.txt  : Stores registered usernames and passwords

Example User Database:
    user.txt stores credentials in the format:

        username,password

Example:
        Omkar,123
        demo,123qwe
        Shreyas,12

Suitable For:
    - Demonstrating TCP socket programming
    - Learning multi-threaded server design
    - Understanding client-server chat systems

===============================================================================
*/
#include "main.h"

// Arrays to store usernames and passwords
char usernames[MAX_USERS][30];
char passwords[MAX_USERS][20];
int user_count = 0;

// Linked list head for online users
online_user *head = NULL;

// Mutex for thread synchronization
pthread_mutex_t lock;

// Function declarations
void load_users();
void handle_signup(int sockfd, userInfo rec);
int handle_login(int sockfd, userInfo rec);
void add_online_user(char *username, int sockfd);
void remove_online_user(int sockfd);
void send_online_users(int sockfd);
void broadcast_join(char *username, int new_sockfd);
void broadcast_leave(char *username, int sockfd);
void send_private_message(userInfo rec);
void send_group_message(userInfo rec);
void crash_handler(int sig);

// Thread function to handle each client
void *handle_client(void *arg)
{
    int sock_fd = *(int *)arg;
    free(arg);

    userInfo rec;
    char logged_user[30] = "";

    while (1)
    {
        // Receive data from client
        if (recv(sock_fd, &rec, sizeof(rec), 0) <= 0)
            break;

        // Handle login request
        if (rec.option == LOGIN)
        {
            if (handle_login(sock_fd, rec))
                strcpy(logged_user, rec.username);
        }

        // Handle signup request
        else if (rec.option == SIGNUP)
        {
            handle_signup(sock_fd, rec);
        }

        // Handle logout
        else if (rec.option == LOGOUT)
        {
            break;
        }

        // Handle private message
        else if (rec.option == SINGLE_CHAT)
        {
            send_private_message(rec);
        }

        // Handle group message
        else if (rec.option == GROUP_CHAT)
        {
            send_group_message(rec);
        }
    }

    // Notify others that user left
    if (strlen(logged_user) > 0)
        broadcast_leave(logged_user, sock_fd);

    remove_online_user(sock_fd);
    close(sock_fd);

    return NULL;
}

// Main server function
int main()
{
    int server_fd;
    struct sockaddr_in serv_addr;

    printf("Server is waiting...\n");

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // Listen for clients
    listen(server_fd, NO_OF_CLIENTS);

    // Load users from file
    load_users();

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Register signal handlers
    signal(SIGINT, crash_handler);
    signal(SIGTSTP, crash_handler);

    while (1)
    {
        // Accept new client
        int *new_sock_fd = malloc(sizeof(int));
        *new_sock_fd = accept(server_fd, NULL, NULL);

        if (*new_sock_fd < 0)
        {
            perror("accept");
            free(new_sock_fd);
            continue;
        }

        printf("Client Connected\n");

        // Create thread for client
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, new_sock_fd);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&lock);
    close(server_fd);
}

// Handle signup request
void handle_signup(int sockfd, userInfo rec)
{
    status_t reply;

    // Check if user already exists
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(usernames[i], rec.username) == 0)
        {
            reply = USER_ALREADY_EXISTS;
            send(sockfd, &reply, sizeof(reply), 0);
            return;
        }
    }

    // Store new user
    strcpy(usernames[user_count], rec.username);
    strcpy(passwords[user_count], rec.password);
    user_count++;

    // Save to file
    int fd = open("user.txt", O_WRONLY | O_APPEND);
    char new_entry[100];
    sprintf(new_entry, "%s,%s\n", rec.username, rec.password);
    write(fd, new_entry, strlen(new_entry));
    close(fd);

    reply = SIGNUP_SUCCESS;
    send(sockfd, &reply, sizeof(reply), 0);
}

// Handle login request
int handle_login(int sockfd, userInfo rec)
{
    status_t reply;

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(usernames[i], rec.username) == 0)
        {
            if (strcmp(passwords[i], rec.password) == 0)
            {
                reply = LOGIN_SUCCESS;

                // Add user to online list
                add_online_user(rec.username, sockfd);

                send(sockfd, &reply, sizeof(reply), 0);

                // Send list of online users
                send_online_users(sockfd);

                // Notify others
                broadcast_join(rec.username, sockfd);

                return 1;
            }
            else
            {
                reply = WRONG_PASSWORD;
                send(sockfd, &reply, sizeof(reply), 0);
                return 0;
            }
        }
    }

    reply = USERNAME_NOT_FOUND;
    send(sockfd, &reply, sizeof(reply), 0);

    return 0;
}

// Add user to online linked list
void add_online_user(char *username, int sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *new_node = malloc(sizeof(online_user));
    strcpy(new_node->username, username);
    new_node->sockfd = sockfd;

    new_node->next = head;
    head = new_node;

    pthread_mutex_unlock(&lock);
}

// Remove user from online list
void remove_online_user(int sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;
    online_user *prev = NULL;

    while (temp)
    {
        if (temp->sockfd == sockfd)
        {
            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            break;
        }

        prev = temp;
        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);
}

// Send online users to newly connected client
void send_online_users(int sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;
    online_packet packet;

    while (temp)
    {
        memset(&packet, 0, sizeof(packet));
        strcpy(packet.name, temp->username);
        packet.type = PACKET_LIST;
        packet.stop = 0;

        send(sockfd, &packet, sizeof(packet), 0);

        temp = temp->next;
    }

    // Send stop packet
    memset(&packet, 0, sizeof(packet));
    packet.type = PACKET_LIST;
    packet.stop = 1;

    send(sockfd, &packet, sizeof(packet), 0);

    pthread_mutex_unlock(&lock);
}

// Notify users that someone joined
void broadcast_join(char *username, int new_sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;
    online_packet packet;

    memset(&packet, 0, sizeof(packet));
    strcpy(packet.name, username);
    packet.type = PACKET_JOIN;

    while (temp)
    {
        if (temp->sockfd != new_sockfd)
            send(temp->sockfd, &packet, sizeof(packet), 0);

        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);
}

// Notify users that someone left
void broadcast_leave(char *username, int sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;
    online_packet packet;

    memset(&packet, 0, sizeof(packet));
    strcpy(packet.name, username);
    packet.type = PACKET_LEAVE;

    while (temp)
    {
        if (temp->sockfd != sockfd)
            send(temp->sockfd, &packet, sizeof(packet), 0);

        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);
}

// Load users from file
void load_users()
{
    int fd;
    char ch;
    char line[100];
    int i = 0;

    fd = open("user.txt", O_RDONLY | O_CREAT, 0644);

    while (read(fd, &ch, 1) > 0)
    {
        if (ch != '\n')
        {
            line[i++] = ch;
        }
        else
        {
            line[i] = '\0';
            i = 0;

            char *token = strtok(line, ",");
            if (token)
                strcpy(usernames[user_count], token);

            token = strtok(NULL, ",");
            if (token)
                strcpy(passwords[user_count], token);

            user_count++;
        }
    }

    close(fd);
}

// Send private message
void send_private_message(userInfo rec)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;
    int found = 0;

    while (temp)
    {
        if (strcmp(temp->username, rec.target) == 0)
        {
            send(temp->sockfd, &rec, sizeof(rec), 0);
            found = 1;
            break;
        }

        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);
}

// Send message to all users
void send_group_message(userInfo rec)
{
    pthread_mutex_lock(&lock);

    online_user *temp = head;

    while (temp)
    {
        if (strcmp(temp->username, rec.username) != 0)
            send(temp->sockfd, &rec, sizeof(rec), 0);

        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);
}

// Handle server crash
void crash_handler(int sig)
{
    printf("\nServer crashed!\n");

    pthread_mutex_lock(&lock);

    online_user *temp = head;

    userInfo msg;
    memset(&msg, 0, sizeof(msg));

    strcpy(msg.username, "SERVER");
    strcpy(msg.message, "Server has crashed!");
    msg.option = GROUP_CHAT;

    while (temp)
    {
        send(temp->sockfd, &msg, sizeof(msg), 0);
        close(temp->sockfd);
        temp = temp->next;
    }

    pthread_mutex_unlock(&lock);

    exit(0);
}