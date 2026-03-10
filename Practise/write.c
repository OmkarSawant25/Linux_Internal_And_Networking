#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    
    fd = open("file.txt", O_WRONLY | O_CREAT, 0644);

    write(fd, "Hello Linux\n", 12);

    close(fd);
}