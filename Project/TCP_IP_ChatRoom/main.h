#ifndef MAIN_H
#define MAIN_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

// #define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_IP_ADDRESS "192.168.1.102"
#define SERVER_PORT 6333
#define NO_OF_CLIENTS 10
#define MAX_USERS 100

// ANSI color escape sequences for terminal output
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct
{
    int sockfd;
    struct sockaddr_in server_addr;
} client_t;

typedef struct
{
    char username[30]; // sender
    char password[20];
    char target[30];   // receiver username
    char message[256]; // chat message
    int option;        // LOGIN / SIGNUP / LOGOUT / SINGLE_CHAT / GROUP_CHAT
} userInfo;

typedef struct online_user
{
    char username[30];
    int sockfd;
    struct online_user *next;
} online_user;

typedef enum
{
    LOGIN,
    SIGNUP,
    LOGOUT,
    SINGLE_CHAT,
    GROUP_CHAT,
    LOGIN_SUCCESS,
    SIGNUP_SUCCESS,
    USER_ALREADY_EXISTS,
    USERNAME_NOT_FOUND,
    WRONG_PASSWORD
} status_t;

typedef enum
{
    PACKET_LIST = 0,
    PACKET_JOIN,
    PACKET_LEAVE
} packet_type_t;

typedef struct
{
    char name[30];
    int stop;           // 0 = continue , 1 = end
    packet_type_t type; // LIST / JOIN / LEAVE
} online_packet;

// void load_users();
// void handle_signup(int sockfd, userInfo rec);
// int handle_login(int sockfd, userInfo rec);
// void add_online_user(char *username, int sockfd);
// void remove_online_user(int sockfd);
// void send_online_users(int sockfd);
// void broadcast_join(char *username, int new_sockfd);
// void broadcast_leave(char *username, int sockfd);
// void send_private_message(userInfo rec);

#endif