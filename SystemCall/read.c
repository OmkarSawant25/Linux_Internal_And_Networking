#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("file.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 0;
    }

    printf("FD value is => %d\n", fd);

    char buff[10];
    int ret = 0;

    while ((ret = read(fd, buff, 5)) > 0)
    {
        buff[ret] = '\0'; // null terminate
        printf("The return value => %d\n", ret);
        printf("The string read => %s\n", buff);
    }

    return 0;
}

// open() → returns file descriptor
// fd == -1 → error
// errno → reason for failure
// read() → returns number of bytes read
// read() returns 0 → EOF
