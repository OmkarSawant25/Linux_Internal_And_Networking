#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid = fork();
    if (pid > 0)
    {
        // Parent Process
        printf("Parent Process\n");
        printf("Parent %d\n", getpid());
        printf("Child %d\n", pid);

    }
    else if (pid == 0)
    {
        // Child Process
        printf("Child Process\n");
        printf("Parent %d\n", getppid());
        printf("Child %d\n", getpid());
    }
    else
    {
        perror("fork");
    }
}