#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> // stat, fstat, fchmod

int main()
{
    int fd = open("file.txt", O_RDONLY);
    struct stat var;
    fstat(fd, &var);
    printf("Size of File is -> %ld\n", var.st_size);
    // printf("Permission of File is -> %d\n", var.st_mode);

}