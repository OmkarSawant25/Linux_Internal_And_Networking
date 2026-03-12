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
    // check no arguments
    if (argc <= 1)
    {
        printf("Error: No arguments passed\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }

    // check insufficient arguments
    if (argc < 4)
    {
        printf("Error: Insufficient arguments passed\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }

    int next_index = -1;

    // find position of '|' and split commands
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            argv[i] = NULL;     // terminate first command arguments
            next_index = i + 1; // second command starts from here
            break;
        }
    }

    // if '|' not found
    if (next_index == -1)
    {
        printf("Error: '|' is not present\n");
        printf("Usage: ./pipe <command1 > '|' <command2> \n");
        return -1;
    }

    int pipe_fd[2];
    pipe(pipe_fd); // create pipe

    int pid1 = fork();
    int status;

    if (pid1 > 0) // parent
    {
        int pid2 = fork(); // create second child

        if (pid2 > 0) // parent after creating both children
        {
            close(pipe_fd[0]); // close unused read end
            close(pipe_fd[1]); // close unused write end

            waitpid(pid2, &status, 0); // wait for child2
        }
        else if (pid2 == 0) // child2 (reader)
        {
            close(pipe_fd[1]);   // close write end
            dup2(pipe_fd[0], 0); // make pipe read end as STDIN

            execvp(argv[next_index], argv + next_index); // execute second command
        }

        waitpid(pid1, &status, 0); // wait for child1
    }
    else if (pid1 == 0) // child1 (writer)
    {
        close(pipe_fd[0]);   // close read end
        dup2(pipe_fd[1], 1); // make pipe write end as STDOUT

        execvp(argv[1], argv + 1); // execute first command
    }
    else
    {
        perror("fork");
    }
}
