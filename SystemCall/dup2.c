#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int backup = dup(1); // backup stdout

    dup2(file, 1); // stdout -> file
    printf("Hello");
    fflush(stdout);

    dup2(backup, 1); // restore stdout
    printf("World\n");

    close(file);
    close(backup);

    return 0;
}
