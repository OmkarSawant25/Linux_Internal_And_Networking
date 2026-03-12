/*
   Name        : Omkar Ashok Sawant
   Program     : A14 – WAP to print the address which is causing Segmentation Fault

   Description :
   This program demonstrates signal handling using the sigaction()
   system call. It registers a custom signal handler for the SIGSEGV
   (segmentation fault) signal and prints the memory address which
   caused the fault using information available in struct siginfo.

   The program intentionally creates a segmentation fault condition.
   When the fault occurs, the registered signal handler gets executed,
   retrieves the faulting address from the siginfo structure, prints
   the address on the standard output, and terminates the program.

   The program demonstrates:
   1. Handling signals using sigaction()
   2. Working of SIGSEGV signal
   3. Usage of sa_sigaction handler with SA_SIGINFO flag
   4. Accessing fault address using struct siginfo
   5. Creating and handling runtime errors safely

   Problem Statement :
   Write a C program to handle segmentation fault using sigaction().
   Register a signal handler that prints the memory address which
   caused the segmentation fault using struct siginfo. The program
   should intentionally generate a segmentation fault, display the
   faulting address, and then exit gracefully.
*/

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
