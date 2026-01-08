/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A08 – WAP to create three child processes from the same parent

   Description :
   This program demonstrates the creation of multiple child
   processes from a single parent process using the fork()
   system call. The parent process waits for all three child
   processes to terminate and prints the exit status of each
   child process.

   The program demonstrates:
   1. Creation of multiple child processes using fork()
   2. Understanding parent–child process relationship
   3. Waiting for multiple child processes using wait()
   4. Retrieval and display of exit status of child processes
   5. Proper handling of process termination

   Problem Statement :
   Write a C program to create three child processes from the
   same parent process. The parent process must wait for all
   three child processes to complete execution and display
   the exit status of each child process when they terminate.

   OUTPUT :
   ./three_child

   Child 1 with pid 2020 created
   Child 2 with pid 2021 created
   Child 3 with pid 2022 created
   Child 2020 is terminated with code 0
   Child 2021 is terminated with code 0
   Child 2022 is terminated with code 0
*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid, pid1, pid2;
    pid = fork();
    int status;
    if (pid > 0)
    {
        printf("A child1 created with pid %d \n", pid);
        pid1 = fork();
        if (pid1 > 0)
        {
            printf("A child2 created with pid %d \n", pid1);
            pid2 = fork();
            if (pid2 > 0)
            {
                printf("A child3 created with pid %d \n", pid2);
                while (waitpid(pid, &status, WNOHANG) == 0);
                if (WIFEXITED(status))
                {
                    printf("child1 %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
                }
            }
            else if (pid2 == 0)
            {
                sleep(3);
            }
            while (waitpid(pid1, &status, WNOHANG) == 0);
            if (WIFEXITED(status))
            {
                printf("child2 %d terminated normally with exit status %d parent terminating\n", pid1, WEXITSTATUS(status));
            }
        }
        else if (pid1 == 0)
        {
            sleep(3);
        }
        while (waitpid(pid2, &status, WNOHANG) == 0);
        if (WIFEXITED(status))
        {
            printf("child3 %d terminated normally with exit status %d parent terminating\n", pid2, WEXITSTATUS(status));
        }
    }
    else if (pid == 0)
    {
        sleep(3);
    }
    return 0;
}