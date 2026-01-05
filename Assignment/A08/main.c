#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid, pid1, pid2;
    pid = fork();
    int status;
    if (pid > 0)
    {
        printf("A child1 created with pid %d \n", pid);
        pid1 = fork();
        if (pid1 > 0)
        {
            printf("A child2 created with pid %d \n", pid1);
            pid2 = fork();
            if (pid2 > 0)
            {
                printf("A child3 created with pid %d \n", pid2);
                while (waitpid(pid, &status, WNOHANG) == 0);
                if (WIFEXITED(status))
                {
                    printf("child1 %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
                }
            }
            else if (pid2 == 0)
            {
                sleep(3);
            }
            while (waitpid(pid1, &status, WNOHANG) == 0);
            if (WIFEXITED(status))
            {
                printf("child2 %d terminated normally with exit status %d parent terminating\n", pid1, WEXITSTATUS(status));
            }
        }
        else if (pid1 == 0)
        {
            sleep(3);
        }
        while (waitpid(pid2, &status, WNOHANG) == 0);
        if (WIFEXITED(status))
        {
            printf("child3 %d terminated normally with exit status %d parent terminating\n", pid2, WEXITSTATUS(status));
        }
    }
    else if (pid == 0)
    {
        sleep(3);
    }
    return 0;
}