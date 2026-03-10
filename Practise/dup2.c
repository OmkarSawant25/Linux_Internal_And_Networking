#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);

    dup2(fd, 1);

    printf("Hello Linux\n");
}