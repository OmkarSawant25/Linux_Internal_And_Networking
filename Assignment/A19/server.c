#include "header.h"

#define SERVER_PORT 5001
#define SERVER_IP "127.0.0.1"

int main()
{
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    input_command_packet new;
    data_packet data_var;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server waiting...\n");

    recvfrom(sock_fd, &new, sizeof(new), 0, (struct sockaddr *)&client_addr, &client_len);

    // Execute command N times
    for (int i = 0; i < new.no_of_times; i++)
    {
        int pid = fork();

        if (pid == 0)
        {
            int fd = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            char temp[100];
            strcpy(temp, new.command);

            char *argv[20];
            int j = 0;

            char *token = strtok(temp, " ");
            while (token != NULL)
            {
                argv[j++] = token;
                token = strtok(NULL, " ");
            }
            argv[j] = NULL;

            execvp(argv[0], argv);
            exit(1);
        }
        else if(pid > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("fork");
        }
    }

    // Send file content
    int file_fd = open("output.txt", O_RDONLY);
    int packet = 1;
    int b_size;

    while ((b_size = read(file_fd, data_var.data, 64)) > 0)
    {
        data_var.packet_no = packet;
        data_var.flag = 1;
        data_var.size = b_size;

        while (1)
        {
            sendto(sock_fd, &data_var, sizeof(data_var), 0,
                   (struct sockaddr *)&client_addr, client_len);

            ack_packet ack;
            recvfrom(sock_fd, &ack, sizeof(ack), 0,
                     (struct sockaddr *)&client_addr, &client_len);

            if (ack.packet_no == packet)
                break;
        }

        packet++;
    }

    // Termination packet
    data_var.packet_no = packet;
    data_var.flag = 0;
    data_var.size = 0;

    sendto(sock_fd, &data_var, sizeof(data_var), 0, (struct sockaddr *)&client_addr, client_len);

    close(file_fd);
    close(sock_fd);
}
