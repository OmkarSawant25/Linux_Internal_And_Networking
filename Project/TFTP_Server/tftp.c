/* Common file for server & client */

#include "tftp.h"
#include <fcntl.h>

void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{
    tftp_packet packet, ack_packet;
    int fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    uint16_t block_number = 1;
    int bytes_read;

    while (1)
    {
        memset(&packet, 0, sizeof(packet));

        bytes_read = read(fd, packet.body.data_packet.data, 512);

        if (bytes_read < 0)
        {
            perror("Read error");
            close(fd);
            return;
        }

        packet.opcode = htons(DATA);
        packet.body.data_packet.block_number = htons(block_number);
        
        sendto(sockfd,
               &packet,
               4 + bytes_read,
               0,
               (struct sockaddr *)&client_addr,
               client_len);

        printf("Sent DATA block %d (%d bytes)\n",
               block_number,
               bytes_read);

        memset(&ack_packet, 0, sizeof(ack_packet));

        recvfrom(sockfd,
                 &ack_packet,
                 sizeof(ack_packet),
                 0,
                 (struct sockaddr *)&client_addr,
                 &client_len);

        while (ntohs(ack_packet.opcode) != ACK ||
               ntohs(ack_packet.body.ack_packet.block_number) != block_number)
        {
            printf("Resending block %d\n", block_number);

            sendto(sockfd,
                   &packet,
                   4 + bytes_read,
                   0,
                   (struct sockaddr *)&client_addr,
                   client_len);

            memset(&ack_packet, 0, sizeof(ack_packet));

            recvfrom(sockfd,
                     &ack_packet,
                     sizeof(ack_packet),
                     0,
                     (struct sockaddr *)&client_addr,
                     &client_len);
        }

        if (bytes_read < 512)
            break;

        block_number++;
    }

    printf("File transfer completed successfully\n");
    close(fd);
}

void receive_file(int sockfd,
                  struct sockaddr_in client_addr,
                  socklen_t client_len,
                  char *filename)
{
    tftp_packet packet, ack_packet;

   int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    uint16_t expected_block = 1;
    int n;

    while (1)
    {
        memset(&packet, 0, sizeof(packet));

        // Receive packet and store actual size
        n = recvfrom(sockfd,
                     &packet,
                     sizeof(packet),
                     0,
                     (struct sockaddr *)&client_addr,
                     &client_len);

        if (n < 0)
        {
            perror("recvfrom failed");
            close(fd);
            return;
        }

        if (ntohs(packet.opcode) != DATA)
        {
            printf("Invalid packet received\n");
            close(fd);
            return;
        }

        uint16_t block_number =
            ntohs(packet.body.data_packet.block_number);

        if (block_number == expected_block)
        {
            // Actual data size = total received - 4 bytes header
            int data_size = n - 4;

            write(fd,
                  packet.body.data_packet.data,
                  data_size);

            printf("Received DATA block %d (%d bytes)\n",
                   block_number, data_size);

            // Send ACK
            memset(&ack_packet, 0, sizeof(ack_packet));
            ack_packet.opcode = htons(ACK);
            ack_packet.body.ack_packet.block_number =
                htons(block_number);

            sendto(sockfd,
                   &ack_packet,
                   sizeof(ack_packet),
                   0,
                   (struct sockaddr *)&client_addr,
                   client_len);

            expected_block++;

            // If last block
            if (data_size < 512)
            {
                break;
            }
        }
    }

    printf("File received successfully\n");
    close(fd);
}
