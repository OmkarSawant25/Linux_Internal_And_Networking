// #include <stdio.h>
// #include <signal.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>

// void my_handler(int signum, siginfo_t *info, void *ptr)
// {
//     if (signum == SIGINT)
//     {
//         printf("SIGINT received\n");
//     }

//     int n = 5;
//     while (n--)
//     {
//         printf("Inside handler\n");
//         sleep(1);
//     }
// }

// int main()
// {
//     struct sigaction new_act;

//     printf("Process %d waiting for signal.. press ctrl + c from terminal.\n", getpid());

//     memset(&new_act, 0, sizeof(new_act));

//     new_act.sa_sigaction = my_handler;
//     new_act.sa_flags = SA_SIGINFO;

//     // Initialize signal set
//     sigemptyset(&new_act.sa_mask);
//     sigaddset(&new_act.sa_mask, SIGUSR1);
//     sigaddset(&new_act.sa_mask, SIGTERM);
//     sigaddset(&new_act.sa_mask, SIGABRT);

//     // Register handler
//     sigaction(SIGINT, &new_act, NULL);

//     while (1)
//     {
//         pause();
//     }

//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

void my_handler(int sig, siginfo_t *info, void *p)
{
    printf("Signal triggered-> signal no %d \n", info->si_signo);
    for (int i = 0; i < 15; sleep(1), i++)
    {
        printf("Inside the Handler %d...\n", i + 1);
    }
}

int main()
{
    printf(" Process %d waiting for signal.. press ctrl + c from terminal.\n", getpid());
    struct sigaction sa;
    sa.sa_sigaction = &my_handler;
    sa.sa_flags = SA_SIGINFO;

    sigaddset(&sa.sa_mask, SIGTSTP);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGABRT);
    sigaddset(&sa.sa_mask, SIGINT);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    while (1)
    {
        pause();
    }
}