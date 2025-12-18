#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    int fd = open("new.txt", O_WRONLY);

    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            perror("open");
            return 0;
        }
    }

    printf("The return FD value is %d\n", fd);

    return 0;
}

// errno tells why open() failed
// perror() prints system error message
// EEXIST → file already exists