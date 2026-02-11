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
#define SERVER_PORT 6001

int main()
{
    int sock_fd, data_sock_fd;
    struct sockaddr_in serv_addr;
    int num1, num2, result = 0;

    // socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("INFO: created server socket\n");

    // bind
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int flag = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // listen()
    listen(sock_fd, 1);
    printf("LISTENING: plus server is listening\n");

    // accept
    data_sock_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
    printf("ACCEPTED: plus server accepted connection\n");

    // recv num1
    if (recv(data_sock_fd, &num1, sizeof(num1), 0) <= 0)
    {
        perror("recv");
        close(data_sock_fd);
        return 1;
    }

    // recv num2
    if (recv(data_sock_fd, &num2, sizeof(num2), 0) <= 0)
    {
        perror("recv");
        close(data_sock_fd);
        return 1;
    }

    result = num1 + num2;

    printf("Client data : %d  %d  %c\n", num1, num2, '+');
    printf("Result is %d\n", result);
    printf("Sending result to client...\n");

    send(data_sock_fd, &result, sizeof(result), 0);
    close(data_sock_fd);
    close(sock_fd);
}
