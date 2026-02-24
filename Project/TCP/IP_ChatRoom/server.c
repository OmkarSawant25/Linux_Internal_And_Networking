#include "main.h"

online_user *head = NULL;
pthread_mutex_t lock; // Step 1

void *handle_client(void *arg)
{
    int sock_fd = *(int *)arg;
    free(arg);
    userInfo rec;
    while (1)
    {
        if (recv(sock_fd, &rec, sizeof(rec), 0) <= 0)
            break;
        if (rec.option == LOGIN)
            handle_login(sock_fd, rec);
        else if (rec.option == SIGNUP)
            handle_signup(sock_fd, rec);
    }
    remove_online_user(sock_fd);
    close(sock_fd);
    return NULL;
}

int main()
{
    int server_fd, data_sock_fd;
    struct sockaddr_in serv_addr;

    printf("Server is waiting...\n");

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    listen(server_fd, NO_OF_CLIENTS);

    load_users();

    while (1)
    {
        int *new_sock_fd = malloc(sizeof(int));
        *new_sock_fd = accept(server_fd, NULL, NULL);
        if (*new_sock_fd < 0)
        {
            perror("accept");
            free(new_sock_fd);
            continue;
        }
        printf("Client Connected\n");

        pthread_t tid;
        pthread_mutex_init(&lock, NULL); // Step 2
        pthread_create(&tid, NULL, handle_client, new_sock_fd);
        pthread_detach(tid);
        pthread_mutex_destroy(&lock); // Step 5
    }
    close(server_fd);
}

void load_users()
{
    int fd;
    char ch;
    char line[100];
    int i = 0;

    fd = open("user.txt", O_RDONLY | O_CREAT, 0644);

    while (read(fd, &ch, 1) > 0)
    {
        if (ch != '\n')
        {
            line[i++] = ch;
        }
        else
        {
            line[i] = '\0';
            i = 0;

            char *token = strtok(line, ",");
            if (token)
                strcpy(usernames[user_count], token);

            token = strtok(NULL, ",");
            if (token)
                strcpy(passwords[user_count], token);

            user_count++;
        }
    }

    close(fd);
}

void handle_signup(int sockfd, userInfo rec)
{
    status_t reply;

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(usernames[i], rec.username) == 0)
        {
            reply = USER_ALREADY_EXISTS;
            send(sockfd, &reply, sizeof(reply), 0);
            return;
        }
    }

    strcpy(usernames[user_count], rec.username);
    strcpy(passwords[user_count], rec.password);
    user_count++;

    int fd = open("user.txt", O_WRONLY | O_APPEND);
    char new_entry[100];
    sprintf(new_entry, "%s,%s\n", rec.username, rec.password);
    write(fd, new_entry, strlen(new_entry));
    close(fd);

    reply = SIGNUP_SUCCESS;
    send(sockfd, &reply, sizeof(reply), 0);
}

void handle_login(int sockfd, userInfo rec)
{
    status_t reply;

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(usernames[i], rec.username) == 0)
        {
            if (strcmp(passwords[i], rec.password) == 0)
            {
                reply = LOGIN_SUCCESS;
                add_online_user(rec.username, sockfd);
                send(sockfd, &reply, sizeof(reply), 0);
                send_online_users(sockfd);
                return;
            }
            else
            {
                reply = WRONG_PASSWORD;
                send(sockfd, &reply, sizeof(reply), 0);
                return;
            }
        }
    }

    reply = USERNAME_NOT_FOUND;
    send(sockfd, &reply, sizeof(reply), 0);
}

void add_online_user(char *username, int sockfd)
{
    pthread_mutex_lock(&lock);

    online_user *new_node = malloc(sizeof(online_user));
    strcpy(new_node->username, username);
    new_node->sockfd = sockfd;
    new_node->next = head;
    head = new_node;

    pthread_mutex_unlock(&lock);
}

void remove_online_user(int sockfd)
{
    pthread_mutex_lock(&lock); // Step 3

    online_user *temp = head;
    online_user *prev = NULL;

    while (temp)
    {
        if (temp->sockfd == sockfd)
        {
            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            break;
        }

        prev = temp;
        temp = temp->next;
    }

    pthread_mutex_unlock(&lock); // Step 4
}

void send_online_users(int sockfd)
{
    pthread_mutex_lock(&lock); // Step 3

    online_user *temp = head;
    online_packet packet;

    while (temp)
    {
        memset(&packet, 0, sizeof(packet));
        strcpy(packet.name, temp->username);
        packet.stop = 0;

        send(sockfd, &packet, sizeof(packet), 0);

        temp = temp->next;
    }

    memset(&packet, 0, sizeof(packet));
    packet.stop = 1;
    send(sockfd, &packet, sizeof(packet), 0);

    pthread_mutex_unlock(&lock); // Step 4
}