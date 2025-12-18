#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd;
    fd = open("new.txt", O_CREAT | O_WRONLY, 0644);
    char buff[50];
    scanf("%s", buff);
    write(fd, buff, strlen(buff));
    close(fd);
}