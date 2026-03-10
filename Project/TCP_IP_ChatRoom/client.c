/*
===============================================================================
Project Title : TCP/IP Chat Room System (Client)
Name          : Omkar Ashok Sawant
Batch Id      : 25021C_309
Date          : 07/03/2026
Language      : C Programming
Protocol      : TCP (Transmission Control Protocol)
===============================================================================

Project Overview:
    This program implements the client-side of a multi-user TCP/IP chat
    room system. The client connects to a central chat server and allows
    users to communicate with other connected clients through private
    messages or group chat.

    The client provides login and signup functionality and supports
    real-time communication with multiple users through the server.

Supported Operations:
        - Login        → Authenticate existing user
        - Sign-Up      → Register a new user
        - Single Chat  → Send private message to a specific user
        - Group Chat   → Send message to all online users
        - Logout       → Exit from chat room

Features:
    1. Server Connection:
       - Establishes TCP connection with the chat server.

    2. User Authentication:
       - Supports Login and Signup system.

    3. Online User Notification:
       - Displays users joining and leaving the chat room.

    4. Private Messaging:
       - Allows communication between two specific users.

    5. Group Messaging:
       - Broadcast messages to all online users.

    6. Multi-threaded Listening:
       - Uses a separate thread to receive messages from server.

    7. Signal Handling:
       - Gracefully handles client exit using Ctrl+C or Ctrl+Z.

File Structure:
    - client.c  : Client implementation and chat interface
    - main.h    : Shared structures, macros, and protocol definitions

Extensibility:
    - GUI-based chat interface can be added.
    - File transfer support can be integrated.
    - Message encryption can be implemented for secure communication.

Suitable For:
    - Students learning TCP socket programming in C
    - Demonstrating client-server architecture

===============================================================================
*/

#include "main.h"

// Function declarations
void handle_login(client_t *client);
void handle_signup(client_t *client);
void receive_online_users(client_t *client);
void chat_menu(client_t *client);
void single_chat(client_t *client);
void group_chat(client_t *client);
void *listen_server(void *arg);

// Stores current logged-in username
char current_user[30];

// Global socket for signal handler
int global_sockfd;

// Signal handler when client exits using Ctrl+C or Ctrl+Z
void client_crash_handler(int sig)
{
    printf(ANSI_COLOR_RED "\n[System] : Client exiting due to interrupt signal.\n" ANSI_COLOR_RESET);

    // Send logout message to server
    userInfo logout_user;
    memset(&logout_user, 0, sizeof(logout_user));
    logout_user.option = LOGOUT;

    send(global_sockfd, &logout_user, sizeof(logout_user), 0);

    // Close socket
    close(global_sockfd);
    exit(0);
}

// Thread function to listen messages from server
void *listen_server(void *arg)
{
    int sockfd = *(int *)arg;
    free(arg);

    while (1)
    {
        char buffer[1024];

        // Receive data from server
        int n = recv(sockfd, buffer, sizeof(buffer), 0);

        // If server disconnects
        if (n <= 0)
        {
            printf(ANSI_COLOR_RED "\n[System] : Server disconnected.\n" ANSI_COLOR_RESET);
            close(sockfd);
            raise(SIGKILL);
            return NULL;
        }

        // If packet contains online user notification
        if (n == sizeof(online_packet))
        {
            online_packet *packet = (online_packet *)buffer;

            switch (packet->type)
            {
            case PACKET_JOIN:
                printf(ANSI_COLOR_YELLOW "\n[Notification] : User %s is ONLINE\n" ANSI_COLOR_RESET, packet->name);
                break;

            case PACKET_LEAVE:
                printf(ANSI_COLOR_YELLOW "\n[Notification] : User %s is OFFLINE\n" ANSI_COLOR_RESET, packet->name);
                break;

            default:
                printf(ANSI_COLOR_RED "Unknown online packet received\n" ANSI_COLOR_RESET);
                break;
            }
        }

        // If packet contains chat message
        else if (n == sizeof(userInfo))
        {
            userInfo *msg = (userInfo *)buffer;

            switch (msg->option)
            {
            case SINGLE_CHAT:
                printf(ANSI_COLOR_GREEN "\n[Private] %s : %s" ANSI_COLOR_RESET, msg->username, msg->message);
                break;

            case GROUP_CHAT:
                printf(ANSI_COLOR_MAGENTA "\n[Group] %s : %s" ANSI_COLOR_RESET, msg->username, msg->message);
                break;

            case LOGOUT:
                printf(ANSI_COLOR_YELLOW "\n[Notification] : User %s has logged out.\n" ANSI_COLOR_RESET, msg->username);
                break;

            default:
                printf(ANSI_COLOR_RED "Unknown chat packet received\n" ANSI_COLOR_RESET);
                break;
            }
        }

        fflush(stdout);
    }

    return NULL;
}

// Main function
// Creates socket and connects to server
int main()
{
    client_t client;
    userInfo new_user;
    status_t response;

    printf(ANSI_COLOR_YELLOW "\n=========================================\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "      MULTI-CLIENT TCP/IP CHAT ROOM\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "=========================================\n" ANSI_COLOR_RESET);

    // Create TCP socket
    client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client.sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Set server address
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(SERVER_PORT);
    client.server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    // Connect to server
    if (connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr)) != 0)
    {
        perror("connect");
        close(client.sockfd);
        exit(1);
    }

    printf(ANSI_COLOR_MAGENTA "\n[System] : Connected to Chat Room Server.\n" ANSI_COLOR_RESET);

    // Store socket globally
    global_sockfd = client.sockfd;

    // Register signal handlers
    signal(SIGINT, client_crash_handler);
    signal(SIGTSTP, client_crash_handler);

    // Login / Signup menu
    while (1)
    {
        int choice;

        printf("\n1. Login\n2. Sign-Up\nEnter your choice : ");
        scanf("%d", &choice);

        if (choice == 1)
            handle_login(&client);
        else if (choice == 2)
            handle_signup(&client);
        else
            printf(ANSI_COLOR_RED "Invalid choice!\n" ANSI_COLOR_RESET);
    }

    return 0;
}

// Handles user login
void handle_login(client_t *client)
{
    userInfo new_user;
    status_t response;

    new_user.option = LOGIN;

    printf(ANSI_COLOR_BLUE "Enter username : " ANSI_COLOR_RESET);
    scanf("%s", new_user.username);

    printf(ANSI_COLOR_BLUE "Enter password : " ANSI_COLOR_RESET);
    scanf("%s", new_user.password);

    // Send login request to server
    send(client->sockfd, &new_user, sizeof(new_user), 0);

    // Receive server response
    if (recv(client->sockfd, &response, sizeof(response), 0) <= 0)
    {
        printf(ANSI_COLOR_RED "Server disconnected\n" ANSI_COLOR_RESET);
        exit(0);
    }

    if (response == LOGIN_SUCCESS)
    {
        printf(ANSI_COLOR_GREEN "Login Successful!\n" ANSI_COLOR_RESET);

        strcpy(current_user, new_user.username);

        // Receive online users
        receive_online_users(client);

        // Create thread to listen server messages
        pthread_t tid;
        int *sock_ptr = malloc(sizeof(int));
        *sock_ptr = client->sockfd;

        pthread_create(&tid, NULL, listen_server, sock_ptr);

        // Open chat menu
        chat_menu(client);
    }
    else if (response == USERNAME_NOT_FOUND)
    {
        printf(ANSI_COLOR_RED "Username not found!\n" ANSI_COLOR_RESET);
    }
    else if (response == WRONG_PASSWORD)
    {
        printf(ANSI_COLOR_RED "Wrong password!\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf(ANSI_COLOR_RED "Login failed!\n" ANSI_COLOR_RESET);
    }
}

// Handles user signup
void handle_signup(client_t *client)
{
    userInfo new_user;
    status_t response;

    new_user.option = SIGNUP;

    printf(ANSI_COLOR_BLUE "Enter username : " ANSI_COLOR_RESET);
    scanf("%s", new_user.username);

    printf(ANSI_COLOR_BLUE "Enter password : " ANSI_COLOR_RESET);
    scanf("%s", new_user.password);

    // Send signup request
    send(client->sockfd, &new_user, sizeof(new_user), 0);

    // Receive response
    if (recv(client->sockfd, &response, sizeof(response), 0) <= 0)
    {
        printf(ANSI_COLOR_RED "Server disconnected\n" ANSI_COLOR_RESET);
        exit(0);
    }

    if (response == SIGNUP_SUCCESS)
        printf(ANSI_COLOR_GREEN "Signup Successful!\n" ANSI_COLOR_RESET);
    else if (response == USER_ALREADY_EXISTS)
        printf(ANSI_COLOR_RED "User already exists!\n" ANSI_COLOR_RESET);
    else
        printf(ANSI_COLOR_RED "Signup Failed!\n" ANSI_COLOR_RESET);
}

// Receives list of online users
void receive_online_users(client_t *client)
{
    online_packet packet;

    printf(ANSI_COLOR_CYAN "\n========= Online Users =========\n" ANSI_COLOR_RESET);

    while (1)
    {
        if (recv(client->sockfd, &packet, sizeof(packet), 0) <= 0)
        {
            printf(ANSI_COLOR_RED "Server disconnected\n" ANSI_COLOR_RESET);
            exit(0);
        }

        if (packet.stop == 1)
            break;

        if (packet.type == PACKET_LIST)
        {
            printf(ANSI_COLOR_YELLOW "[Info] : User %s is Online\n" ANSI_COLOR_RESET, packet.name);
        }
    }
}

// Chat menu after login
void chat_menu(client_t *client)
{
    while (1)
    {
        int chat_choice;

        printf("\n1. Single Chat");
        printf("\n2. Group Chat");
        printf("\n3. Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &chat_choice);

        if (chat_choice == 1)
            single_chat(client);
        else if (chat_choice == 2)
            group_chat(client);
        else if (chat_choice == 3)
        {
            userInfo logout_user;
            logout_user.option = LOGOUT;

            send(client->sockfd, &logout_user, sizeof(logout_user), 0);

            printf(ANSI_COLOR_GREEN "\n[System] : You have logged out successfully.\n" ANSI_COLOR_RESET);

            close(client->sockfd);
            exit(0);
        }
        else
        {
            printf(ANSI_COLOR_RED "Invalid choice\n" ANSI_COLOR_RESET);
        }
    }
}

// Send private message
void single_chat(client_t *client)
{
    userInfo new_user;

    memset(&new_user, 0, sizeof(new_user));
    new_user.option = SINGLE_CHAT;

    strcpy(new_user.username, current_user);

    printf("Enter username to chat with: ");
    scanf("%s", new_user.target);

    printf("Enter message: ");
    scanf(" %[^\n]", new_user.message);

    send(client->sockfd, &new_user, sizeof(new_user), 0);
}

// Send group message
void group_chat(client_t *client)
{
    userInfo new_user;

    memset(&new_user, 0, sizeof(new_user));
    new_user.option = GROUP_CHAT;

    strcpy(new_user.username, current_user);
    strcpy(new_user.target, "GROUP");

    printf("Enter group message: ");
    scanf(" %[^\n]", new_user.message);

    send(client->sockfd, &new_user, sizeof(new_user), 0);
}