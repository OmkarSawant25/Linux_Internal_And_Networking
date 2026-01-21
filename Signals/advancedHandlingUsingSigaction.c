#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void my_handler(int signum, siginfo_t *info, void *context)
{
    printf("\nSignal received: %d\n", signum);
    printf("Sending process PID: %d\n", info->si_pid);
}

int main()
{
    struct sigaction new_act;

    printf("Current Process ID: %d\n", getpid());

    /* Clear structure */
    memset(&new_act, 0, sizeof(new_act));

    /* Register handler */
    new_act.sa_sigaction = my_handler;
    new_act.sa_flags = SA_SIGINFO;

    /* Register SIGINT (Ctrl + C) */
    sigaction(SIGINT, &new_act, NULL);

    /* Wait for signal */
    while (1)
    {
        pause();
    }

    return 0;
}