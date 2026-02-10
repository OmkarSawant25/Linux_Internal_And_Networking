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
#define SERVER_PORT 6002

int main()
{
    int sock_fd, data_sock_fd;
    struct sockaddr_in serv_addr;
    int num1, num2, result = 0;

    // socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // bind
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // listen()
    listen(sock_fd, 1);

    // accept
    data_sock_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);

    printf("Inside Server of Substraction\n");

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

    result = num1 - num2;
    send(data_sock_fd, &result, sizeof(result), 0);
    close(data_sock_fd);
    close(sock_fd);
}
