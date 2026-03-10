#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[100];

    fd = open("file.txt", O_RDONLY);

    read(fd, buffer, 100);

    write(1, buffer, 100);
    

    close(fd);
}