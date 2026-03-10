#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child Process\n");
        // printf("pid is %d\n", getpid());
    }
    else
    {
        int pid2 = wait(NULL);
        // printf("Pid is %d\n", pid2);
        printf("Parent Process\n");
    }
}