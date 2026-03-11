// #include <stdio.h>
// #include <unistd.h>
// #include <string.h>

// int main()
// {
//     int fd[2];
//     pipe(fd);

//     int pid = fork();
//     if (pid > 0)
//     {
//         // Parent
//         close(fd[0]); // close read end
//         char msg[] = "Omkar";
//         write(fd[1], msg, strlen(msg) + 1);
//     }
//     else if (pid == 0)
//     {
//         // Child
//         close(fd[1]); // close write end
//         char buff[100];
//         read(fd[0], buff, sizeof(buff));

//         printf("Message from Parent is %s\n", buff);
//     }
//     else
//     {
//         perror("fork");
//     }
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid > 0)
    {
        // Parent
        close(fd[1]); // close write end
        char buff[100];
        read(fd[0], buff, sizeof(buff));

        printf("Message from Child is %s\n", buff);
    }
    else if (pid == 0)
    {
        // Child

        close(fd[0]); // close read end
        char msg[] = "Omkar";
        write(fd[1], msg, strlen(msg) + 1);
    }
    else
    {
        perror("fork");
    }
    return 0;
}