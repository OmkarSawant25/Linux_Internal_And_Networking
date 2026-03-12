#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// SIGCHLD handler using SA_SIGINFO
void my_handler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGCHLD)
    {
        // child exit status available in siginfo
        printf("Child terminated with exit code %d\n", info->si_status);
    }
}

int main()
{
    struct sigaction new_act;

    memset(&new_act, 0, sizeof(new_act));

    new_act.sa_sigaction = my_handler;            // register handler
    new_act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT; // avoid zombie automatically

    sigemptyset(&new_act.sa_mask); // initialize mask

    sigaction(SIGCHLD, &new_act, NULL);

    int pid = fork();

    if (pid > 0)
    {
        // parent keeps running
        printf("Parent PID %d waiting...\n", getpid());
        while (1)
            pause();
    }
    else if (pid == 0)
    {
        // child process
        printf("Child PID %d running...\n", getpid());
        sleep(5);
        exit(0);
    }
    else
    {
        perror("fork");
    }
}
