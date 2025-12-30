#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid, status;
    pid = fork();

    if (pid > 0)
    {
        printf("A child created with pid %d\n", pid);
        int ret;
        while ((ret = waitpid(pid, &status, WNOHANG)) == 0)
        {
            printf("Parent is running\n");
            sleep(1);
        }

        if (ret == pid && WIFEXITED(status))
        {
            printf("Child %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
        }
        else
        {
            printf("Child %d terminated abnormally\n", pid);
        }
    }
    else if (pid == 0)
    {
        sleep(5);
    }
    else
    {
        perror("fork");
    }
}