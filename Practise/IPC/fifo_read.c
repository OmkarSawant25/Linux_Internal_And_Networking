#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    mkfifo("my_fifo", 0666);
    int fd = open("my_fifo", O_RDONLY);

    char buff[100];
    read(fd, buff, sizeof(buff));
    printf("Received message is %s\n", buff);

    close(fd);
}