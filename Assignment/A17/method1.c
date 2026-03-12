/*
   Name        : Omkar Ashok Sawant
   Program     : A17 – WAP to avoid child becoming zombie using signal handlers

   Description :
   This program demonstrates how to prevent creation of zombie
   processes using signal handling techniques. When a child
   process terminates, the parent receives the SIGCHLD signal.
   By handling this signal properly, the parent can collect the
   exit status of the child asynchronously and prevent the child
   from entering zombie state.

   Two different methods are implemented. In Method 1, a custom
   signal handler for SIGCHLD is registered using sigaction().
   Inside the handler, wait() or waitpid() is used to reap the
   terminated child process and print its exit status.

   In Method 2, the SA_NOCLDWAIT flag is used in struct sigaction
   to automatically prevent zombie creation by instructing the
   kernel not to retain the child’s exit status.

   The program demonstrates:
   1. Creation of child process using fork()
   2. Handling SIGCHLD signal asynchronously
   3. Avoiding zombie processes using wait()/waitpid() in handler
   4. Usage of sa_flags like SA_NOCLDWAIT
   5. Retrieval and display of child exit status
   6. Understanding process termination behaviour

   Problem Statement :
   Write two separate C programs to avoid child process becoming
   zombie using signal handling. In the first method, handle the
   SIGCHLD signal and reap the child process inside the handler.
   In the second method, use SA_NOCLDWAIT flag in sigaction() to
   prevent zombie creation.
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// SIGCHLD handler to reap terminated child
void my_handler(int signum)
{
    int status;
    pid_t pid;

    // reap all terminated children
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
            printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
        else
            printf("Child %d terminated abnormally\n", pid);
    }
}

int main()
{
    signal(SIGCHLD, my_handler); // register SIGCHLD handler

    int pid = fork();

    if (pid > 0)
    {
        // parent process keeps running
        printf("Parent PID %d waiting...\n", getpid());
        while (1)
            pause(); // wait for signals
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