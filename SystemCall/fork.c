#include <stdio.h>
#include <unistd.h>   // fork(), getpid(), getppid()
#include <sys/types.h>

int main()
{
    int ret = fork();

    if (ret > 0)
    {
        // Parent process
        printf("Parent PID = %d\n", getpid());
    }
    else if (ret == 0)
    {
        // Child process
        printf("Child PID = %d\n", getpid());
        printf("Parent PID = %d\n", getppid());
    }
    else
    {
        perror("fork");
    }

    return 0;
}
