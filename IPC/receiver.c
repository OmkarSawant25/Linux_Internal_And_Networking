#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int fd;
    char buf[50];

    // open FIFO in read mode
    fd = open("/tmp/my_fifo", O_RDONLY);

    // read data from FIFO
    read(fd, buf, sizeof(buf));

    // print received data
    printf("The data read from FIFO is: %s\n", buf);

    return 0;
}

