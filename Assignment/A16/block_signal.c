#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void my_handler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGINT)
    {
        printf("SIGINT received\n");
    }

    int n = 5;
    while (n--)
    {
        printf("Inside handler\n");
        sleep(1);
    }
}

int main()
{
    struct sigaction new_act;

    printf("Process %d waiting for signal.. press ctrl + c from terminal.\n", getpid());

    memset(&new_act, 0, sizeof(new_act));

    new_act.sa_sigaction = my_handler;
    new_act.sa_flags = SA_SIGINFO;

    // Initialize signal set
    sigemptyset(&new_act.sa_mask);
    sigaddset(&new_act.sa_mask, SIGUSR1);
    sigaddset(&new_act.sa_mask, SIGTERM);
    sigaddset(&new_act.sa_mask, SIGABRT);

    // Register handler
    sigaction(SIGINT, &new_act, NULL);

    while (1)
    {
        pause();
    }

    return 0;
}
