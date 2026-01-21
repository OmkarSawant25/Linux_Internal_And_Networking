#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void my_handler(int signum)
{
    printf("\nReceived signal number: %d\n", signum);

    if (signum == SIGINT)
    {
        printf("No Lab\n");
        signal(SIGINT, SIG_IGN);
    }
    else if (signum == SIGTSTP)
    {
        printf("Lab is there\n");
        signal(SIGTSTP, SIG_DFL);
    }
}

int main()
{
    printf("Process ID: %d\n", getpid());

    // Registering signals
    signal(SIGINT, my_handler);  // Ctrl + C
    signal(SIGTSTP, my_handler); // Ctrl + Z

    while (1); // Infinite loop

    return 0;
}
