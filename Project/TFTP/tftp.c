/* Common file for server & client */

#include "tftp.h"

void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{

    printf("Inside Send File\n");
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    int block = 1;

    while (1)
    {
        tftp_packet packet;
        memset(&packet, 0, sizeof(packet));

        packet.opcode = htons(DATA);
        packet.body.data_packet.block_number = htons(block);

        int bytes_read = read(fd, packet.body.data_packet.data, 512);

        if (bytes_read < 0)
        {
            perror("read failed");
            break;
        }

        int packet_size = bytes_read + 4; // header + data

        sendto(sockfd, &packet, packet_size, 0, (struct sockaddr *)&client_addr, client_len);

        printf("Sent DATA %d\n", block);

        // Wait for ACK
        tftp_packet ack_packet;
        recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, &client_len);

        int ack_opcode = ntohs(ack_packet.opcode);

        if (ack_opcode == ACK)
        {
            int ack_block = ntohs(ack_packet.body.ack_packet.block_number);
            printf("Received ACK %d\n", ack_block);
        }

        block++;

        if (bytes_read < 512)
            break; // Last packet
    }

    close(fd);
    printf("File sent successfully\n");
}

void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{
    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    int expected_block = 1;

    while (1)
    {
        tftp_packet packet;
        memset(&packet, 0, sizeof(packet));

        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        if (n < 0)
        {
            perror("recvfrom failed");
            break;
        }

        int opcode = ntohs(packet.opcode);

        if (opcode == DATA)
        {
            int block = ntohs(packet.body.data_packet.block_number);

            if (block == expected_block)
            {
                int data_size = n - 4; // Remove header (2+2)

                write(fd, packet.body.data_packet.data, data_size);

                // Send ACK
                tftp_packet ack_packet;
                memset(&ack_packet, 0, sizeof(ack_packet));

                ack_packet.opcode = htons(ACK);
                ack_packet.body.ack_packet.block_number = htons(block);

                sendto(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, client_len);

                printf("ACK %d sent\n", block);

                expected_block++;

                if (data_size < 512)
                    break; // Last packet
            }
        }
    }

    close(fd);
    printf("File received successfully\n");
}
