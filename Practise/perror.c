#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd = open("abc.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Open failed");
    }
}