#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    /* open the file.txt file in write mode */
    int fd = open("file.txt", O_WRONLY);

    /* print the return FD value */
    printf("The return FD value is => %d\n", fd);

    return 0;
}
