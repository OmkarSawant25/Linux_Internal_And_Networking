#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    int fd = open("my_fifo", O_WRONLY);

    char msg[] = "Omkar Ashok Sawant";
    write(fd, msg, strlen(msg) + 1);

    close(fd);
}