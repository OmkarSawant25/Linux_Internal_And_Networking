#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define SERVER_PORT_ADD 6001
#define SERVER_PORT_SUB 6002
#define SERVER_PORT_DIV 6003
#define SERVER_PORT_MOD 6004
#define SERVER_PORT_MUL 6005
#define SERVER_LENGTH 5

int main()
{
    int sock_fd, data_sock_fd;
    struct sockaddr_in serv_addr;
    char opr;

    // create a socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error: Could not create socket\n");
        return 1;
    }
    else
    {
        printf("INFO: created server socket\n");
    }

    // bind
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(sock_fd, SERVER_LENGTH);
    printf("LISTENING: relay server is listening\n");

    data_sock_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
    printf("ACCEPTED: relay server accepted connection\n");

    // recv
    if (recv(data_sock_fd, &opr, sizeof(opr), 0) <= 0)
    {
        perror("recv");
        close(data_sock_fd);
        return 1;
    }

    int port;

    if (opr == '+')
        port = SERVER_PORT_ADD;
    else if (opr == '-')
        port = SERVER_PORT_SUB;
    else if (opr == '*')
        port = SERVER_PORT_MUL;
    else if (opr == '/')
        port = SERVER_PORT_DIV;
    else if (opr == '%')
        port = SERVER_PORT_MOD;

    send(data_sock_fd, &port, sizeof(port), 0);
    close(data_sock_fd);
}

