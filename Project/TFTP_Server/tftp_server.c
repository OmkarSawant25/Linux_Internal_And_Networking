#include "tftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    tftp_packet packet;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("TFTP Server listening on port %d...\n", PORT);

    while (1)
    {
        client_len = sizeof(client_addr); // 🔹 Reset length each time

        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        if (n < 0)
        {
            perror("Receive failed");
            continue;
        }

        handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet)
{
    int opcode = ntohs(packet->opcode);

    /* ================= WRQ ================= */
    if (opcode == WRQ)
    {
        char *filename = packet->body.request.filename;
        printf("WRQ received for file: %s\n", filename);

        int fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, 0644);

        if (fd == -1)
        {
            if (errno == EEXIST)
            {
                fd = open(filename, O_WRONLY | O_TRUNC);
                if (fd == -1)
                {
                    perror("Failed to truncate file");
                    return;
                }
            }
            else
            {
                perror("File open failed");
                return;
            }
        }

        close(fd);

        // Send ACK(0)
        tftp_packet ack;
        memset(&ack, 0, sizeof(ack));

        ack.opcode = htons(ACK);
        ack.body.ack_packet.block_number = htons(0);

        sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);

        printf("Sent ACK(0). Ready to receive file.\n");

        receive_file(sockfd, client_addr, client_len, filename);
    }

    /* ================= RRQ ================= */
    else if (opcode == RRQ)
    {
        char *filename = packet->body.request.filename;
        printf("RRQ received for file: %s\n", filename);

        int fd = open(filename, O_RDONLY);

        if (fd < 0)
        {
            tftp_packet error_packet;
            memset(&error_packet, 0, sizeof(error_packet));

            error_packet.opcode = htons(ERROR);
            error_packet.body.error_packet.error_code = htons(1);

            strcpy(error_packet.body.error_packet.error_msg, "File does not exist");

            int err_len = 4 + strlen(error_packet.body.error_packet.error_msg) + 1;

            sendto(sockfd, &error_packet, err_len, 0, (struct sockaddr *)&client_addr, client_len);

            printf("File not found. ERROR sent.\n");
            return;
        }

        close(fd);
        printf("File found. Starting transfer...\n");
        send_file(sockfd, client_addr, client_len, filename);
    }
}
