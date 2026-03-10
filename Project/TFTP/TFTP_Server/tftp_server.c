/*
===============================================================================
Project Title : TFTP File Transfer System (Server)
Name          : Omkar Ashok Sawant
Batch Id      : 25021C_309
Date          : 07/03/2026
Language      : C Programming
Protocol      : UDP (TFTP - Trivial File Transfer Protocol)
===============================================================================

Project Overview:
    This program implements the server-side of a Trivial File Transfer
    Protocol (TFTP) system using UDP sockets in C. The server listens
    on a predefined port and processes requests from multiple clients
    for uploading or downloading files.

    The server handles two primary requests:
        - WRQ (Write Request) -> Client uploads file to server
        - RRQ (Read Request)  -> Client downloads file from server

    File transfer occurs using DATA packets and acknowledgements (ACK).

Features:
    1. Server Initialization:
       - Creates UDP socket.
       - Binds socket to port 6969.
       - Listens continuously for incoming client requests.

    2. WRQ Handling (File Upload):
       - Receives write request from client.
       - Sends ACK(0) to confirm readiness.
       - Receives file data block-by-block.

    3. RRQ Handling (File Download):
       - Validates requested file.
       - Sends file in DATA packets to client.
       - Waits for acknowledgements.

    4. Transfer Modes:
       - Supports default, octet, and netascii modes.

    5. Error Handling:
       - Sends ERROR packet if requested file does not exist.
       - Handles file creation and truncation safely.

File Structure:
    - tftp_server.c : Server implementation and request handling.
    - tftp.c        : Common file transfer functions (send_file, receive_file).
    - tftp.h        : Packet structures, macros, and function prototypes.

Extensibility:
    - Can be extended to support multiple clients using threads.
    - Timeout and retransmission mechanisms can be added.
    - Logging system and authentication can be implemented.

Suitable For:
    - Students learning computer networks and socket programming.
    - Demonstrating UDP-based protocol implementation in C.

===============================================================================
*/

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

// Function to process client request (RRQ or WRQ)
void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

// Global variables for transfer mode and block size
char mode[10];
int data_size;

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
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_port = htons(PORT);       // Convert port to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connection from any IP

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    printf("TFTP Server listening on port %d...\n", PORT);

    // Server continuously waits for client requests
    while (1)
    {
        client_len = sizeof(client_addr); // Reset client length before receiving

        // Receive packet from client
        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0)
        {
            perror("Receive failed");
            continue;
        }
        // Process received request
        handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet)
{
    // Get opcode from packet (convert from network to host format)
    int opcode = ntohs(packet->opcode);

    /* ================= WRITE REQUEST (WRQ) ================= */
    if (opcode == WRQ)
    {
        // Extract filename from request
        char *filename = packet->body.request.filename;

        printf("WRQ received for file: %s\n", filename);
        printf("Mode received: %s\n", packet->body.request.mode);

        // Store transfer mode
        strcpy(mode, packet->body.request.mode);

        // Set block size according to transfer mode
        if (strcmp(mode, "default") == 0)
            data_size = 512;
        else if (strcmp(mode, "octet") == 0)
            data_size = 1;
        else if (strcmp(mode, "netascii") == 0)
            data_size = 512;
        else
            data_size = 512;

        printf("Server block size set to %d\n", data_size);

        // Try to create a new file
        int fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, 0644);

        if (fd == -1)
        {
            // If file already exists
            if (errno == EEXIST)
            {
                // Open file and truncate existing content
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

        // Prepare ACK packet with block number 0
        tftp_packet ack;
        memset(&ack, 0, sizeof(ack));

        ack.opcode = htons(ACK);
        ack.body.ack_packet.block_number = htons(0);

        // Send ACK(0) to client indicating server is ready
        sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("Sent ACK(0). Ready to receive file.\n");

        // Start receiving file from client
        receive_file(sockfd, client_addr, client_len, filename);
    }

    /* ================= READ REQUEST (RRQ) ================= */
    else if (opcode == RRQ)
    {
        // Extract filename requested by client
        char *filename = packet->body.request.filename;
        printf("Mode received: %s\n", packet->body.request.mode);

        // Store transfer mode
        strcpy(mode, packet->body.request.mode);

        // Set block size based on mode
        if (strcmp(mode, "default") == 0)
            data_size = 512;
        else if (strcmp(mode, "octet") == 0)
            data_size = 1;
        else if (strcmp(mode, "netascii") == 0)
            data_size = 512;
        else
            data_size = 512;

        printf("Server block size set to %d\n", data_size);
        printf("RRQ received for file: %s\n", filename);

        // Check if requested file exists
        int fd = open(filename, O_RDONLY);
        if (fd < 0)
        {
            // Prepare ERROR packet if file not found
            tftp_packet error_packet;
            memset(&error_packet, 0, sizeof(error_packet));
            error_packet.opcode = htons(ERROR);
            error_packet.body.error_packet.error_code = htons(1);
            strcpy(error_packet.body.error_packet.error_msg, "File does not exist");
            int err_len = 4 + strlen(error_packet.body.error_packet.error_msg) + 1;
            // Send error packet to client
            sendto(sockfd, &error_packet, err_len, 0, (struct sockaddr *)&client_addr, client_len);
            printf("File not found. ERROR sent.\n");
            return;
        }
        close(fd);
        printf("File found. Starting transfer...\n");

        // Send file to client
        send_file(sockfd, client_addr, client_len, filename);
    }
}