#include "tftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);
bool validate_file_present(char *file_name);

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    tftp_packet packet;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Set socket timeout option
    // TODO Use setsockopt() to set timeout option

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("TFTP Server listening on port %d...\n", PORT);

    // Main loop to handle incoming requests
    while (1)
    {
        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0)
        {
            perror("Receive failed or timeout occurred");
            continue;
        }

        handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet)
{
    // Extract the TFTP operation (read or write) from the received packet
    // and call send_file or receive_file accordingly
    int opcode = ntohs(packet->opcode);

    if (opcode == WRQ)
    {
        char *filename = packet->body.request.filename;

        printf("WRQ received for file: %s\n", filename);

        int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);

        if (fd < 0)
        {
            perror("File open failed");

            // Send ERROR packet
            tftp_packet error_packet;
            memset(&error_packet, 0, sizeof(error_packet));

            error_packet.opcode = htons(ERROR);
            error_packet.body.error_packet.error_code = htons(2); // Access violation

            strcpy(error_packet.body.error_packet.error_msg, "File open failed");
            sendto(sockfd, &error_packet, sizeof(error_packet), 0, (struct sockaddr *)&client_addr, client_len);
            return;
        }

        printf("File opened successfully\n");
        close(fd);

        // Send ACK block
        tftp_packet ack_packet;
        memset(&ack_packet, 0, sizeof(ack_packet));

        ack_packet.opcode = htons(ACK);
        ack_packet.body.ack_packet.block_number = htons(0);

        sendto(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, client_len);

        printf("Sent ACK block 0\n");
    }
}

bool validate_file_present(char *file_name)
{
    int fd = open(file_name, O_RDONLY);
    if (fd == -1)
        return false;

    close(fd);
    return true;
}
