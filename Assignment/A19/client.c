#include "header.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5001

int main()
{
    int sock_fd;
    struct sockaddr_in server_addr;

    input_command_packet new;
    data_packet data_var;
    ack_packet ack;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Enter any standard LS command : ");
    scanf(" %[^\n]", new.command);

    printf("Enter no.of times to be executed : ");
    scanf("%d", &new.no_of_times);

    socklen_t server_len = sizeof(server_addr);
    sendto(sock_fd, &new, sizeof(new), 0,(struct sockaddr *)&server_addr, server_len);

    while (1)
    {
        int r_size = recvfrom(sock_fd, &data_var, sizeof(data_var), 0, (struct sockaddr *)&server_addr, &server_len);

        if (r_size <= 0)
            break;

        write(1, data_var.data, data_var.size);

        ack.packet_no = data_var.packet_no;

        sendto(sock_fd, &ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, server_len);

        if (data_var.flag == 0)
            break;
    }
    close(sock_fd);
}
