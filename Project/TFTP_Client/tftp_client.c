#include "tftp.h"
#include "tftp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

bool validate_ip_address(char *ip);
bool validate_file_present(char *file_name);

int main()
{
    char command[256];
    tftp_client_t client;
    memset(&client, 0, sizeof(client)); // Initialize client structure

    printf("Type help to get supported features\n");

    // Main loop for command-line interface
    while (1)
    {
        printf("tftp> ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Process the command
        process_command(&client, command);
    }

    return 0;
}

// Function to process commands
void process_command(tftp_client_t *client, char *command)
{
    char *cmd = strtok(command, " ");

    if (cmd == NULL)
        return;

    if (strcmp("help", cmd) == 0)
    {
        print_help();
    }
    else if (strcmp("bye", cmd) == 0 || strcmp("quit", cmd) == 0)
    {
        exit(1);
    }
    else if (strcmp("connect", cmd) == 0)
    {
        char *ip = strtok(NULL, " ");
        bool valid = validate_ip_address(ip);
        if (valid)
        {
            connect_to_server(client, ip, PORT);
            printf("INFO: Connected to server\n");
        }
        else
        {
            printf("Usage: connect <ip>\n");
        }
    }
    else if (strcmp("put", cmd) == 0)
    {
        char *file_name = strtok(NULL, " ");
        bool valid_file = validate_file_present(file_name);
        if (valid_file)
        {
            put_file(client, file_name);
        }
        else
        {
            printf("ERROR: File is not present\n");
        }
    }
    else if (strcmp("get", cmd) == 0)
    {
        char *file_name = strtok(NULL, " ");
        get_file(client, file_name);
    }
}

// This function is to initialize socket with given server IP, no packets sent to server in this function
void connect_to_server(tftp_client_t *client, char *ip, int port)
{
    // Create UDP socket
    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Set socket timeout option

    // Set up server address
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(PORT);
    client->server_addr.sin_addr.s_addr = inet_addr(ip);

    strcpy(client->server_ip, ip);
    client->server_ip[strlen(ip)] = '\0';
    client->server_len = sizeof(client->server_addr);
}

void put_file(tftp_client_t *client, char *filename)
{
    if (client->sockfd <= 0)
    {
        printf("Not connected to server\n");
        return;
    }
    send_request(client->sockfd, client->server_addr, filename, WRQ);
}

void get_file(tftp_client_t *client, char *filename)
{
    // Send RRQ and recive file
    send_request(client->sockfd, client->server_addr, filename, RRQ);
}

void disconnect(tftp_client_t *client)
{
    // close fd
}

void send_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    tftp_packet packet;
    memset(&packet, 0, sizeof(packet));

    packet.opcode = htons(opcode);
    strcpy(packet.body.request.filename, filename);

    printf("File name is : %s\n", filename);

    if (opcode == WRQ)
        printf("Sending WRQ to server...\n");
    else
        printf("Sending RRQ to server...\n");

    sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    receive_request(sockfd, server_addr, filename, opcode);
}

void receive_request(int sockfd,
                     struct sockaddr_in server_addr,
                     char *filename,
                     int opcode)
{
    tftp_packet response;
    socklen_t len = sizeof(server_addr);

    /* ================= WRQ ================= */
    if (opcode == WRQ)
    {
        int n = recvfrom(sockfd,
                         &response,
                         sizeof(response),
                         0,
                         (struct sockaddr *)&server_addr,
                         &len);

        if (n < 0)
        {
            perror("recvfrom failed");
            return;
        }

        if (ntohs(response.opcode) == ERROR)
        {
            printf("Server ERROR: %s\n",
                   response.body.error_packet.error_msg);
            return;
        }

        if (ntohs(response.opcode) == ACK &&
            ntohs(response.body.ack_packet.block_number) == 0)
        {
            printf("Server ready. Starting file upload...\n");
            send_file(sockfd, server_addr, len, filename);
        }
        else
        {
            printf("Invalid ACK received\n");
        }
    }

    /* ================= RRQ ================= */
    else if (opcode == RRQ)
    {
        printf("Receiving file...\n");

        /* DO NOT recv here */
        receive_file(sockfd, server_addr, len, filename);

        printf("File download completed\n");
    }
}


void print_help()
{
    printf("connect <server-ip>    : Connect to server\n");
    printf("get <file-name>        : Receive a file from server\n");
    printf("put <file-name>        : Send a file to the server\n");
    printf("mode                   : Transfer mode(s) (octet, netascii or mail)\n");
    printf("bye / quit             : Exit the application\n");
}

bool validate_ip_address(char *ip)
{
    if (ip == NULL)
        return false;

    struct sockaddr_in sa;

    int result = inet_pton(AF_INET, ip, &(sa.sin_addr)); // stores binary IP inside memory I provide

    return (result == 1);
}

bool validate_file_present(char *file_name)
{
    int fd = open(file_name, O_RDONLY);
    if (fd == -1)
        return false;

    close(fd);
    return true;
}