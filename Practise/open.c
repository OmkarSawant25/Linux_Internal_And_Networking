#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd = open("new.txt", O_CREAT | O_WRONLY, 0644);
    printf("%d\n", fd);
}