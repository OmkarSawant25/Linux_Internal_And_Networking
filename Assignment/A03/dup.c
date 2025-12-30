// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/stat.h>

// int main()
// {
//     int file = open("demo.txt", O_WRONLY);
//     int backup = dup(1);    // Taking STDOUT
//     close(1);
//     int new_fd = dup(file);
//     printf("Hello");
//     close(new_fd);
//     dup(backup);
//     printf("World\n");
// }

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int file = open("demo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int backup = dup(1);   // backup stdout

    close(1);
    dup(file);             // stdout -> demo.txt

    printf("Hello");
    fflush(stdout);        

    close(1);
    dup(backup);           // restore stdout

    printf("World\n");
}
