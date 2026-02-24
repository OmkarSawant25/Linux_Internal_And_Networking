#include "main.h"

int main()
{
    client_t client;
    userInfo new_user;
    status_t response;

    // Create socket
    client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client.sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Server address
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(SERVER_PORT);
    client.server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    // Connect
    if (connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr)) != 0)
    {
        perror("connect");
        close(client.sockfd);
        exit(1);
    }

    printf("Connected to server.\n");

    while (1)
    {
        int choice;
        printf("\n1. Login\n2. Sign-Up\nEnter your choice : ");
        scanf("%d", &choice);

        if (choice == 1)
            new_user.option = LOGIN;
        else if (choice == 2)
            new_user.option = SIGNUP;
        else
        {
            printf("Invalid choice!\n");
            continue;
        }

        printf("Enter username : ");
        scanf("%s", new_user.username);

        printf("Enter password : ");
        scanf("%s", new_user.password);

        // Send request
        send(client.sockfd, &new_user, sizeof(new_user), 0);

        // Receive response
        recv(client.sockfd, &response, sizeof(response), 0);
        printf("Server reply : \n");

        // Handle response
        if (response == LOGIN_SUCCESS)
        {
            printf("Message : Login Successful!\n");
            printf("\nOnline Users:\n");

            online_packet packet;

            while (1)
            {
                if (recv(client.sockfd, &packet, sizeof(packet), 0) <= 0)
                {
                    printf("Server disconnected\n");
                    break;
                }

                if (packet.stop == 1)
                    break;

                printf("%s\n", packet.name);
            }

            break;
        }
        else if (response == SIGNUP_SUCCESS)
        {
            printf("Message : Signup Successful!\n");
        }
        else if (response == USER_ALREADY_EXISTS)
        {
            printf("Message : User already exists!\n");
        }
        else if (response == USERNAME_NOT_FOUND)
        {
            printf("Message : Username not found!\n");
        }
        else if (response == WRONG_PASSWORD)
        {
            printf("Message : Wrong password!\n");
        }
    }

    while(1);

    close(client.sockfd);
    return 0;
}