// Parameters
// fd → file descriptor (from open())
// offset → number of bytes to move
// whence → reference point

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("new.txt", O_RDONLY);

    lseek(fd, 6, SEEK_SET);  

    char ch;
    read(fd, &ch, 1);        
    printf("%c\n", ch);

    close(fd);
    return 0;
}
