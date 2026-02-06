/*------------------------------------------------------------------------------ 
 *
 *   Author         : Emertxe (http://www.emertxe.com) 
 *   Date           : Tue 01 Mar 2016 16:18:10 IST
 *   File           : t020.c
 *   Title          : IPC mechanism - Usage of TCP socket - Client 
 *   Description    : Sockets offer interfaces with TCP/IP stack inside Linux 
 *                    Kernel, using which remote connections can be established
 *                    over a network. This program demonstrates various steps
 *                    to establish successful three-way handshake as a client. 
 *
 *----------------------------------------------------------------------------*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERVER_IP_ADDRESS 	"127.0.0.1"
#define SERVER_PORT       	5000
#define CLIENT_BUFF_SIZE	20

int main()
{
		int sock_fd,c_size;
		char client_buff[CLIENT_BUFF_SIZE];
		struct sockaddr_in serv_addr; 

		/* Create a client socket */
		if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
				printf("Error: Could not create socket\n");
				return 1;
		} 

		/* Binding with client socket is optional here */

		/* Populate it with server's address details */
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(SERVER_PORT); 
		serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

		/* Get the message from client */
		printf("Enter the message you want to sent to server:\n");
		scanf("%[^\n]s",client_buff);

		if(connect(sock_fd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
		{
				perror("connect");
				printf("Error connecting to server\n");
				close(sock_fd);
				exit(1);
		}
		else
				printf("Connection successful\n");

		/* Send the message to server */
		c_size = send(sock_fd, client_buff, strlen(client_buff) + 1, 0);

		if(c_size > 0)
				printf("Message sent to server successsfully, please check\n");
		else
				printf("Error: Message send\n");

		close(sock_fd);
}
