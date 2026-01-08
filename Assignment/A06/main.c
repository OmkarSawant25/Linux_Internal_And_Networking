/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A06 – WAP to avoid child becoming zombie without blocking parent

   Description :
   This program demonstrates how to prevent a child process
   from becoming a zombie without blocking the parent process.
   It uses the waitpid() system call with non-blocking options
   to periodically check the termination status of the child
   while allowing the parent process to continue execution.

   The program demonstrates:
   1. Creation of a child process using fork()
   2. Understanding zombie process and its prevention
   3. Non-blocking wait using waitpid() with WNOHANG
   4. Continuous execution of parent while child is running
   5. Detection of child termination and retrieval of exit status
   6. Proper handling of different process states

   Problem Statement :
   Write a C program to create a child process and prevent it
   from becoming a zombie without blocking the parent process.
   Use the waitpid() system call with appropriate arguments to
   periodically check the child's termination status. While the
   child is running, the parent should continue printing a
   message. Once the child terminates, the parent must display
   the child's exit status and terminate gracefully.

   OUTPUT :
   ./nonblock_wait
   A child created with pid 1234
   parent is running
   parent is running
   parent is running ...
   ............................
   Child 1234 terminated normally with exit status 0
   parent terminating
*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid, status;
    pid = fork();

    if (pid > 0)
    {
        printf("A child created with pid %d\n", pid);
        int ret;
        while ((ret = waitpid(pid, &status, WNOHANG)) == 0)
        {
            printf("Parent is running\n");
            sleep(1);
        }

        if (ret == pid && WIFEXITED(status))
        {
            printf("Child %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
        }
        else
        {
            printf("Child %d terminated abnormally\n", pid);
        }
    }
    else if (pid == 0)
    {
        sleep(5);
    }
    else
    {
        perror("fork");
    }
}