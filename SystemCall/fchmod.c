#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    struct stat var;
    int fd = open("demo.txt", O_RDONLY);
    fstat(fd,&var);
    int des_fd = open("demo2.txt", O_RDONLY);
    fchmod(des_fd, var.st_mode);
}