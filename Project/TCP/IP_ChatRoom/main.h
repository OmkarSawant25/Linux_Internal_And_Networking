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

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 6333
#define NO_OF_CLIENTS 10
#define MAX_USERS 100

char usernames[MAX_USERS][30];
char passwords[MAX_USERS][20];
int user_count = 0;

typedef struct
{
    int sockfd;
    struct sockaddr_in server_addr;
} client_t;

typedef struct
{
    char username[30];
    char password[20];
    int option; // LOGIN or SIGNUP
} userInfo;

typedef struct online_user
{
    char username[30];
    int sockfd;
    struct online_user *next;
} online_user;

typedef struct
{
    char name[30];
    int stop;   // 0 = continue , 1 = end
} online_packet;

typedef enum
{
    LOGIN,
    SIGNUP,
    LOGIN_SUCCESS,
    SIGNUP_SUCCESS,
    USER_ALREADY_EXISTS,
    USERNAME_NOT_FOUND,
    WRONG_PASSWORD
} status_t;

void load_users();
void handle_signup(int sockfd, userInfo rec);
void handle_login(int sockfd, userInfo rec);
void add_online_user(char *username, int sockfd);
void remove_online_user(int sockfd);
void send_online_users(int sockfd);

#endif