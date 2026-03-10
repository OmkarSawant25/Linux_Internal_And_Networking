#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pid = fork();
    int status;

    if(pid > 0)
    {
        // Parent
        while(1)
        {
            int ret = waitpid(pid, &status, WNOHANG);
            if(ret == 0)
            {
                printf("Child still running...\n");
            }
            else if(ret > 0)
            {
                printf("Child Terminated\n");
                break;
            }
            sleep(1);
        }

    }
    else if(pid == 0)
    {
        // Child
        printf("Started Child Process\n");
        sleep(5);
        printf("Child Exiting\n");
        return 0;
    }
    else
    {
        perror("fork");
    }
}