/*------------------------------------------------------------------------------ 
 *
 *   Author         : Emertxe (http://www.emertxe.com) 
 *   Date           : Tue 01 Mar 2016 16:18:10 IST
 *   File           : t021.c
 *   Title          : IPC mechanism - Usage of TCP socket - Conrurrent Server
 *   Description    : Sockets offer interfaces with TCP/IP stack inside Linux 
 *                    Kernel, using which remote connections can be established
 *                    over a network. This program demonstrates various steps
 *                    to establish successful three-way handshake as a server
 *                    and handle multiple clients concurrently. 
 *
 *----------------------------------------------------------------------------*/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Change SERVER_IP to appropriate values */
#define SERVER_IP  		"127.0.0.1"
#define SERVER_PORT 	5000
#define SERVER_LENGTH 	10
#define SERVER_BUFF		20

/* Compilation notes:
   ------------------
   1. Compile the server program (gcc -o tcp_conc_server tcp_concurrent_server.c)
   2. Compile the client program (gcc -o tcp_conc_client tcp_concurrent_client.c)
   3. Run the server program (./tcp_conc_server)
   4. Open multiple terminals and run client programs separately (./tcp_conc_client)
   5. The idea is to see the server handling multiple connections concurrently
   6. Start with simple data exchange and make it exchange structure data
   7. This is how a typical client-server based protocol is developed 
   */

int main()
{
		int sock_fd, data_sock_fd,pid;
		struct sockaddr_in serv_addr; 
		char serv_buffer[SERVER_BUFF];

		printf("TCP_SERVER: Concurrent server is waiting...\n");

		/* Create a TCP socket */
		/* This first socket is control connection, all clients connect here */
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);

		/* Bind it to a particular IP address & port) */
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
		serv_addr.sin_port = htons(SERVER_PORT); 

		bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

		/* Wait and listen for incoming connections */
		listen(sock_fd, SERVER_LENGTH);

		/* Get into infinite loop as this is a server */
		while (1)
		{

				/* If there are any, accept and create a new data socket */
				/* Accept returns means TCP threeway handshake (SYN-ACK-SYN) successful
				   The new socket (second socket) is data connecion. For every client 
				   now we need to fork a child in order to handle separate connections */
				data_sock_fd = accept(sock_fd, (struct sockaddr*)NULL, NULL); 

				pid = fork();

				/* Let the child handle data connection */
				if (pid == 0)
				{
						printf("New child forked...awaiting for input from client\n");

						/* Try to get some actual data from client */
						/* TODO: Instead of simple string pass a structure to get a real feel of 
						   each client passing different type of data */
						recv(data_sock_fd, (void *) serv_buffer,SERVER_BUFF,0);

						/* Print the data received from client */
						printf("Here is the client data: %s\n",serv_buffer);

						/* Close the data socket now */
						close(data_sock_fd);

						/* Let the child exit, its done its job now */
						exit(0);

				}
		}

		/* It doesn't get here, incase it gets close the control socket */
		close(sock_fd);
}
