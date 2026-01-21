#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

/* User-defined signal handler */
void my_own_handler(int signum, siginfo_t *info, void *ptr)
{
    printf("Sig no => %d\n", info->si_signo);

    int i = 5;
    while (i--)
    {
        printf("Handler running %d times\n", 10 - i);
        sleep(1);
    }
}

int main()
{
    printf("PID is: %d\n", getpid());

    struct sigaction new_act;

    /* Clear structure */
    memset(&new_act, 0, sizeof(new_act));

    /* Modify the action of SIGINT using sigaction */
    new_act.sa_sigaction = my_own_handler;
    new_act.sa_flags = SA_SIGINFO;

    /* Block SIGINT while handler is executing */
    sigemptyset(&new_act.sa_mask);
    sigaddset(&new_act.sa_mask, SIGINT);  // storing needed signal into sa_mask

    /* Register SIGINT to our handler */
    sigaction(SIGINT, &new_act, NULL);
    sigaction(SIGTSTP, &new_act, NULL);

    /* Waiting for signal */
    while (1)
    {
        pause();
    }

    return 0;
}