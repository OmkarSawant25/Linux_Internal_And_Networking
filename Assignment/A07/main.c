#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Usage: ./a.out <command name>\n");
        return -1;
    }

    int pid1 = fork();
    int status;
    if (pid1 > 0)
    {
        // Parent
        wait(&status);
        if (WIFEXITED(status))
        {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("Child %d terminated abnormally\n", pid1);
        }
    }
    else if (pid1 == 0)
    {
        // Child
        printf("This is the CHILD process, with id %d\n", getpid());
        execvp(argv[1], argv + 1);
    }
    else
    {
        perror("fork");
    }
}