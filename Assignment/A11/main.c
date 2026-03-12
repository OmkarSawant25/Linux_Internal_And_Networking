/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A11 – WAP to implement 'n' pipes to execute 'n+1' commands

   Description :
   This program demonstrates inter-process communication (IPC)
   using multiple pipes to execute a sequence of commands.
   It dynamically creates 'n' pipes to connect 'n+1' related
   child processes, where the output of each command is passed
   as input to the next command in the pipeline.

   The program demonstrates:
   1. Creation of multiple child processes using fork()
   2. Dynamic handling of multiple pipes
   3. Inter-process communication using pipe()
   4. Redirection of standard input and output using dup/dup2()
   5. Execution of multiple commands using exec system calls
   6. Parsing command-line arguments separated by '|'
   7. Synchronization using wait() for all child processes

   Problem Statement :
   Write a C program to implement 'n' pipes to execute 'n+1'
   commands passed through command-line arguments. Each command
   is separated by a pipe ('|') symbol. The first child process
   executes the first command and writes its output to the first
   pipe. Intermediate child processes read input from the
   previous pipe and write output to the next pipe. The last
   child process reads input from the final pipe and executes
   the last command. The parent process should wait for all
   child processes to terminate.

   OUTPUT :
   ./pipe
   Error: No arguments passed, Provide at least one pipe or more
   Usage: ./pipe <command1> '|' <command2>
   Usage: ./pipe <command1> '|' <command2> '|' <command3> ...

   ./pipe ls '|' wc
   5 25 4

   ./pipe ls -l '|' grep "pattern" '|' wc -l
   5 5 25
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int cmd[argc]; // stores starting index of each command
    int count = 1;
    int pipe_count = 0;
    int pipe_fd[2];
    int pid;

    // check no arguments
    if (argc == 1)
    {
        printf("Error: No arguments passed, Provide atleast one pipe or more\n");
        printf("Usage: ./pipe <command1> '|' <command2>\n");
        printf("Usage: ./pipe <command1> '|' <command2> '|' <command3> etc\n");
        return -1;
    }

    cmd[0] = 1; // first command starts from argv[1]

    // find all '|' and split commands
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;       // terminate previous command args
            cmd[count++] = i + 1; // next command start index
            pipe_count++;         // number of pipes
        }
    }

    // loop to create processes for each command
    for (int i = 0; i <= pipe_count; i++)
    {
        if (i != pipe_count)
        {
            pipe(pipe_fd); // create pipe for communication
        }

        pid = fork();

        if (pid > 0)
        {
            // Parent process
            if (i != pipe_count)
            {
                dup2(pipe_fd[0], 0); // next command will read from pipe
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            wait(NULL); // wait child to finish
        }
        else if (pid == 0)
        {
            // Child process
            if (i != pipe_count)
            {
                dup2(pipe_fd[1], 1); // redirect stdout to pipe
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            execvp(argv[cmd[i]], argv + cmd[i]); // execute command
        }
        else
        {
            perror("fork");
        }
    }

    return 0;
}
