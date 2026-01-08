/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A09 – Implement communication between two related processes using one pipe

   Description :
   This program demonstrates inter-process communication (IPC)
   between two related child processes using a single pipe.
   The parent process creates two child processes, where the
   output of the first command is redirected to the input of
   the second command using pipe() and dup()/dup2() system
   calls.

   The program demonstrates:
   1. Creation of multiple child processes using fork()
   2. Inter-process communication using pipe()
   3. Redirection of standard input and output using dup/dup2()
   4. Execution of commands using exec system calls
   5. Parsing command-line arguments separated by '|'
   6. Synchronization using wait() for child process termination

   Problem Statement :
   Write a C program to implement communication between two
   related processes using a single pipe. The program should
   accept two commands from the command line separated by a
   pipe ('|'). The first child process must execute the first
   command and write its output to the pipe. The second child
   process must read input from the pipe and execute the
   second command. The parent process should wait for both
   child processes to terminate.

   OUTPUT :
   ./pipe
   Error: No arguments passed
   Usage: ./pipe <command1> '|' <command2>

   ./pipe ls
   Error: Insufficient arguments passed
   Usage: ./pipe <command1> '|' <command2>

   ./pipe ls '|' wc
   5 25 4

   ./pipe ls -l -a '|' wc -l -w
   10 15
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Error: No arguments passed\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }
    if (argc < 4)
    {
        printf("Error: Insufficient arguments passed\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }

    int next_index = -1;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            argv[i] = NULL;
            next_index = i + 1;
            break;
        }
    }
    if (next_index == -1)
    {
        printf("Error: '|' is not present\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }

    int pipe_fd[2];
    pipe(pipe_fd);

    int pid1 = fork();
    int status;
    if (pid1 > 0)
    {
        // Parent
        int pid2 = fork();
        if (pid2 > 0)
        {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            waitpid(pid2, &status, 0);
        }
        else if (pid2 == 0)
        {
            // Child 2
            close(pipe_fd[1]);
            dup2(pipe_fd[0], 0);
            execvp(argv[next_index], argv + next_index);
        }
        waitpid(pid1, &status, 0);
    }
    else if (pid1 == 0)
    {
        // Child 1
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        execvp(argv[1], argv + 1);
    }
    else
    {
        perror("fork");
    }
}