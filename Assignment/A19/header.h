#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct
{
    char command[100];
    int no_of_times;
} input_command_packet;

typedef struct
{
    char data[64];
    int packet_no;
    int flag;      // 1 = more packets, 0 = last packet
    int size;      // actual bytes
} data_packet;

typedef struct
{
    int packet_no;
} ack_packet;

#endif
