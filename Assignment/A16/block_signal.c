/*
   Name        : Omkar Ashok Sawant
   Program     : A16 – WAP to block certain signals from being received from command line

   Description :
   This program demonstrates signal masking using the sigaction()
   system call. A custom signal handler is registered for a signal
   (for example SIGINT). While the process is executing inside the
   signal handler, certain other signals are temporarily blocked
   using the sa_mask field of struct sigaction.

   A signal set of type sigset_t is created and signals are added
   to the set using sigaddset(). When the handler is active, the
   specified signals will not be delivered to the process. Once
   the handler execution completes, the blocked signals become
   pending and are delivered to the process.

   The program demonstrates:
   1. Registration of signal handler using sigaction()
   2. Creation and manipulation of signal sets using sigset_t
   3. Blocking multiple signals using sa_mask
   4. Behaviour of pending signals during handler execution
   5. Understanding importance of signal masking
   6. Sending signals from another terminal using kill command

   Problem Statement :
   Write a C program to block certain signals while executing
   inside a signal handler. Use sigaction() and sa_mask to
   temporarily block at least three signals. After exiting from
   the handler, the process should respond to the blocked signals.
*/

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
#include <signal.h>

// handler using SA_SIGINFO
void my_handler(int sig, siginfo_t *info, void *p)
{
    printf("Signal triggered -> signal no %d\n", info->si_signo);

    // simulate long handler execution
    for (int i = 0; i < 15; i++)
    {
        printf("Inside handler %d...\n", i + 1);
        sleep(1);
    }
}

int main()
{
    printf("Process %d waiting for signal.. press Ctrl+C\n", getpid());

    struct sigaction sa;

    sigemptyset(&sa.sa_mask); // initialize mask

    // block these signals while handler runs
    sigaddset(&sa.sa_mask, SIGTSTP);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGABRT);

    sa.sa_sigaction = my_handler;
    sa.sa_flags = SA_SIGINFO;

    // register handler
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);

    while (1)
        pause(); // wait for signals
}
