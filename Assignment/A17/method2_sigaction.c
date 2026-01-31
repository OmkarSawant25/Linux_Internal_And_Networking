#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void my_handler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGCHLD)
    {
        printf("Child terminated with exit code %d\n", info->si_status);
    }
}
int main()
{
    struct sigaction new_act;
    memset(&new_act, 0, sizeof(struct sigaction));

    new_act.sa_sigaction = my_handler;
    new_act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT;

    sigaction(SIGCHLD, &new_act, NULL);

    int pid = fork();
    if (pid > 0)
    {
        // Parent
        sleep(10);
    }
    else if (pid == 0)
    {
        // Child
        sleep(5);
    }
    else
    {
        perror("fork");
    }
}