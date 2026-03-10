#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);

    int newfd = dup(fd);

    write(fd, "Hello\n", 6);
    write(newfd, "Linux\n", 6);

    close(fd);
    close(newfd);
}