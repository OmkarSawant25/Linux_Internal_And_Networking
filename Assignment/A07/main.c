/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A07 – WAP to create child process to execute command passed through command line

   Description :
   This program demonstrates the creation of a child process
   and execution of a command passed through command-line
   arguments using exec family system calls. The parent
   process waits for the child to complete and then displays
   the exit status of the child process.

   The program demonstrates:
   1. Creation of a child process using fork()
   2. Execution of external commands using exec system calls
   3. Handling command-line arguments
   4. Error handling for invalid or incorrect commands
   5. Waiting for child termination using wait()
   6. Retrieval and display of child exit status

   Problem Statement :
   Write a C program to create a child process and execute a
   command passed through command-line arguments. If no
   arguments are passed, display a usage message. If an
   invalid command is passed, print an appropriate error
   message. After the child process terminates, the parent
   process should display the exit status of the child.

   OUTPUT :
   ./exe_child
   Usage: ./exe_child args...

   ./exe_child date
   This is the CHILD process, with id 11612
   Wed Apr  4 13:27:19 IST 2012
   Child exited with status 0
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Usage: ./a.out <command name>\n");
        return -1;
    }

    int pid1 = fork();
    int status;
    if (pid1 > 0)
    {
        // Parent
        wait(&status);
        if (WIFEXITED(status))
        {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("Child %d terminated abnormally\n", pid1);
        }
    }
    else if (pid1 == 0)
    {
        // Child
        printf("This is the CHILD process, with id %d\n", getpid());
        execvp(argv[1], argv + 1);
    }
    else
    {
        perror("fork");
    }
}