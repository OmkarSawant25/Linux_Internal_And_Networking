#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

void my_handler(int signum, siginfo_t *info, void *ptr)
{
    // Print error message
    printf("Segmentation fault occurred!\n");

    // Print address causing fault
    printf("Fault address: %p\n", info->si_addr);

    // Exit program
    exit(0);
}

int main()
{
    // Declare sigaction structure
    struct sigaction new_act;

    // Initialize structure
    memset(&new_act, 0, sizeof(new_act));

    // Assign handler function
    new_act.sa_sigaction = my_handler;

    // Enable siginfo
    new_act.sa_flags = SA_SIGINFO;

    // Register SIGSEGV handler
    sigaction(SIGSEGV, &new_act, NULL);

    // Generate segmentation fault
    char *s = "Omkar";
    s[0] = 'Z';

    return 0;
}
