/* Common file for server & client */

#include "tftp.h"
#include <fcntl.h>

// Function to send a file (used by both client & server)
void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{
    tftp_packet packet, ack_packet;

    // Open file in read-only mode
    int fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    int block_number = 1;  // TFTP block numbers start from 1
    int bytes_read;

    // Loop until file sending is complete
    while (1)
    {
        memset(&packet, 0, sizeof(packet)); // Clear packet memory

        // Read maximum 512 bytes from file (TFTP data size)
        bytes_read = read(fd, packet.body.data_packet.data, 512);

        if (bytes_read < 0)
        {
            perror("Read error");
            close(fd);
            return;
        }

        // Set packet type to DATA
        packet.opcode = htons(DATA);

        // Set current block number
        packet.body.data_packet.block_number = htons(block_number);

        // Send DATA packet (4 bytes header + actual data size)
        sendto(sockfd, &packet, 4 + bytes_read, 0, (struct sockaddr *)&client_addr, client_len);

        printf("Sent DATA block %d (%d bytes)\n", block_number, bytes_read);

        // Wait for ACK from receiver
        memset(&ack_packet, 0, sizeof(ack_packet));

        recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, &client_len);

        // If ACK is wrong, resend same block
        while (ntohs(ack_packet.opcode) != ACK || ntohs(ack_packet.body.ack_packet.block_number) != block_number)
        {
            printf("Resending block %d\n", block_number);
            sendto(sockfd, &packet, 4 + bytes_read, 0, (struct sockaddr *)&client_addr, client_len);

            memset(&ack_packet, 0, sizeof(ack_packet));
            recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, &client_len);
        }

        // If last packet (less than 512 bytes), file sending is complete
        if (bytes_read < 512)
            break;

        block_number++;  // Move to next block
    }

    printf("File transfer completed successfully\n");

    close(fd); // Close file
}


// Function to receive a file (used by both client & server)
void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{
    tftp_packet packet, ack_packet;

    // Open file for writing (create if not exists, overwrite if exists)
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0)
    {
        perror("File open failed");
        return;
    }

    int expected_block = 1; // We expect first DATA block to be 1
    int n;

    // Loop until full file is received
    while (1)
    {
        memset(&packet, 0, sizeof(packet)); // Clear packet buffer

        // Receive DATA packet from sender
        n = recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len);

        if (n < 0)
        {
            perror("recvfrom failed");
            close(fd);
            return;
        }

        // Check if received packet is DATA type
        if (ntohs(packet.opcode) != DATA)
        {
            printf("Invalid packet received\n");
            close(fd);
            return;
        }

        int block_number = ntohs(packet.body.data_packet.block_number);

        // If received correct block
        if (block_number == expected_block)
        {
            // Actual data size = total received bytes - 4 bytes header
            int data_size = n - 4;

            // Write received data into file
            write(fd, packet.body.data_packet.data, data_size);

            printf("Received DATA block %d (%d bytes)\n", block_number, data_size);

            // Prepare ACK packet
            memset(&ack_packet, 0, sizeof(ack_packet));

            ack_packet.opcode = htons(ACK);
            ack_packet.body.ack_packet.block_number = htons(block_number);

            // Send ACK back to sender
            sendto(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&client_addr, client_len);

            expected_block++; // Expect next block

            // If last block received (<512 bytes), transfer complete
            if (data_size < 512)
            {
                break;
            }
        }
    }

    printf("File received successfully\n");

    close(fd); // Close file
}
