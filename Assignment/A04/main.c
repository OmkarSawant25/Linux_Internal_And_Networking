/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A04 – WAP to understand advanced file control system calls

   Description :
   This program demonstrates the usage of advanced file control
   system calls using fcntl() to achieve file synchronization
   between two processes. The program creates a parent and a
   child process and ensures mutually exclusive access to a
   shared file using file locking mechanisms.

   The program demonstrates:
   1. Creation of parent and child processes using fork()
   2. File locking and unlocking using fcntl() system call
   3. Synchronization of file access between multiple processes
   4. Blocking behavior when a file is already locked
   5. Safe writing to a shared file by parent and child
   6. Proper error handling for advanced file control calls

   Problem Statement :
   Write a C program to synchronize file access between a
   parent and child process using the fcntl() system call.
   The file name should be passed as a command-line argument.
   Before writing to the file, the process must check whether
   the file is locked. If locked, the process should wait until
   the file becomes available. Once unlocked, the process must
   lock the file, perform its task, write the output to the file,
   and then unlock it. Both parent and child must follow the
   same procedure.

   OUTPUT :
   ./a.out
   → Insufficient arguments
   → Usage:- ./a.out filename

   ./a.out f1.txt
   PARENT PROCESS: locked file
   PARENT PROCESS: writing to file f1.txt
   PARENT PROCESS: unlocked file
   CHILD PROCESS: locked file
   CHILD PROCESS: writing to file f1.txt
   CHILD PROCESS: unlocked file
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // getpid()
#include <sys/types.h>
#include <sys/wait.h> // wait()

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage : ./a.out filename\n");
        return 0;
    }

    close(1);
    // Open the file
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    struct flock var;

    // Create the process
    int pid = fork();

    if (pid > 0)
    {
        // Parent Process
        var.l_type = F_WRLCK;
        var.l_whence = SEEK_SET;
        var.l_start = 0;
        var.l_len = 0;
        var.l_pid = getpid();

        fcntl(fd, F_SETLKW, &var);

        // Print Prime Number
        for (int i = 2; i <= 15; i++)
        {
            int count = 0;
            for (int j = 2; j <= i / 2; j++)
            {
                if ((i % j) == 0)
                {
                    count++;
                }
            }
            if (count == 0)
            {
                printf("%d ", i);
            }
        }
        printf("\n");

        var.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &var);
        wait(NULL);
    }
    else if (pid == 0)
    {
        // Child Process
        var.l_type = F_WRLCK;
        var.l_whence = SEEK_SET;
        var.l_start = 0;
        var.l_len = 0;
        var.l_pid = getpid();

        fcntl(fd, F_SETLKW, &var);

        // Print Fibonacci Series

        int fibo1 = 0, fibo2 = 1, sum = 0, count = 0;

        while (count <= 10)
        {
            printf("%d ", fibo1);
            sum = fibo1 + fibo2;
            fibo1 = fibo2;
            fibo2 = sum;
            count++;
        }
        printf("\n");
        var.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &var);
    }
    else
    {
        perror("Fork");
    }
    return 0;
}