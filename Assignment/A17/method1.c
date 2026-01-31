#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void my_handler(int signum)
{
    int status;
    wait(&status);
    if(WIFEXITED(status))
    {
        printf("Exit status of child process is %d\n", WEXITSTATUS(status));
    }
    else
    {
        printf("Child process terminated abnormally\n");
    }
}

int main()
{
    signal(SIGCHLD, my_handler);
    int pid = fork();

    if(pid > 0)
    {
        // Parent
        sleep(10);
    }
    else if(pid == 0)
    {
        // Child
        sleep(5);
    }
    else
    {
        perror("fork");
    }
}