#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int fd;
    char str[50];
    mkfifo("/tmp/my_fifo", 0644);
    // open FIFO in write mode
    fd = open("/tmp/my_fifo", O_WRONLY);

    printf("Enter the string: ");
    scanf("%[^\n]", str);

    // write data into FIFO
    write(fd, str, sizeof(str));

    return 0;
}

