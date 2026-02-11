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

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 6000

int main()
{
    int num1, num2, size;
    struct sockaddr_in my_addr;
    char opr;

    printf("Enter num1 : ");
    scanf("%d", &num1);
    printf("Enter num2 : ");
    scanf("%d", &num2);
    // getchar();
    printf("Enter arithmetic operator : ");
    scanf(" %c", &opr);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd != -1)
    {
        printf("INFO: Created relay socket\n");
    }
    else
    {
        printf("ERROR: Failed to create relay socket\n");
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVER_PORT);
    my_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    if (connect(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0) // returns 0 if successful
    {
        perror("connect");
        printf("Error connecting to server\n");
        close(sock_fd);
        exit(1);
    }
    else
    {
        printf("SUCCESS: relay connection successful\n");
        printf("SERVER IP %s\n", SERVER_IP_ADDRESS);
        printf("SERVER PORT %d\n", SERVER_PORT);
    }

    size = send(sock_fd, &opr, sizeof(opr), 0);
    if (size > 0)
        printf("Message sent to server successsfully, please check\n");
    else
        printf("Error: Message send\n");

    int arithmetic_port_no;
    recv(sock_fd, &arithmetic_port_no, sizeof(arithmetic_port_no), 0);
    printf("Port no received is %d\n", arithmetic_port_no);

    close(sock_fd);

    int new_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (new_sock_fd != -1)
    {
        printf("INFO: Created server socket\n");
    }
    else
    {
        printf("ERROR: Failed to create server socket\n");
    }

    my_addr.sin_port = htons(arithmetic_port_no);

    if (connect(new_sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0) // returns 0 if successful
    {
        perror("connect");
        printf("Error connecting to server\n");
        close(new_sock_fd);
        exit(1);
    }
    else
    {
        printf("SUCCESS: Server connection successful\n");
    }

    int result = 0;
    send(new_sock_fd, &num1, sizeof(num1), 0);
    send(new_sock_fd, &num2, sizeof(num2), 0);

    recv(new_sock_fd, &result, sizeof(result), 0);
    printf("RESULT: %d\n", result);

    close(new_sock_fd);
}