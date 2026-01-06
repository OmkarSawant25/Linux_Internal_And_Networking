/*
Description

Pre-requisites:
-> Knowledge about system calls, How to read and understand ‘man pages’.
-> Good knowledge about processes and IPC. 
-> Working of pipe & dup system calls.

Objective: 
-> To understand working of pipe between two process. 

Requirements: 
-> Create two child process and execute commands passed from command-line arguments 
-> Each command is separated by a '|' (pipe) character. 
-> First child execute first command (with or without options) and pass o/p to next. 
-> Second child executes second command (after '|') will reads I/p from first cmd. 
-> Parent will wait for both child process to finish .

Sample execution: 
1. ./pipe (No arguments)
    Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2>   

2. ./pipe ls  
    Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2>   

3. ./pipe ls '|' wc
    5 25 4. 

4. ./pipe ls ­ -l -­a '|' wc -­l ­-w 
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