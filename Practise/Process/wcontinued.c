#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        printf("Child running\n");
        sleep(2);

        printf("Child stopping itself\n");
        raise(SIGSTOP);

        printf("Child resumed\n");
        sleep(2);
    }
    else
    {
        waitpid(pid, &status, WUNTRACED);

        if (WIFSTOPPED(status))
        {
            printf("Parent: Child stopped\n");
        }

        sleep(2);

        kill(pid, SIGCONT); // resume child

        waitpid(pid, &status, WCONTINUED);

        if (WIFCONTINUED(status))
        {
            printf("Parent: Child resumed\n");
        }
    }
}