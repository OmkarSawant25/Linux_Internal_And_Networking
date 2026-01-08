/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A10 – Implement communication between three related processes using two pipes

   Description :
   This program demonstrates inter-process communication (IPC)
   between three related child processes using two pipes.
   The parent process creates three child processes where the
   output of the first command is passed to the second command,
   and the output of the second command is passed to the third
   command using pipe(), dup(), and dup2() system calls.

   The program demonstrates:
   1. Creation of multiple child processes using fork()
   2. Inter-process communication using multiple pipes
   3. Redirection of standard input and output using dup/dup2()
   4. Execution of commands using exec system calls
   5. Parsing of command-line arguments separated by '|'
   6. Synchronization using wait() for child process termination

   Problem Statement :
   Write a C program to implement communication between three
   related processes using two pipes. The program should accept
   three commands from the command line separated by pipe ('|')
   symbols. The first child process must execute the first
   command and write its output to the first pipe. The second
   child process must read from the first pipe and write its
   output to the second pipe. The third child process must read
   input from the second pipe and execute the third command.
   The parent process should wait for all child processes to
   terminate.

   OUTPUT :
   ./three_pipes
   Error: No arguments passed
   Usage: ./three_pipes <command1> '|' <command2> '|' <command3>

   ./three_pipes ls -l '|' grep "pattern" '|' wc -l
   5 5 25
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
		printf("Usage: ./three_pipes <command1> '|' <command2> '|' <command3>\n");
		return -1;
	}
	if (argc < 6)
	{
		printf("Error: Insufficient arguments passed\n");
		printf("Usage: ./three_pipes <command1> '|' <command2> '|' <command3>\n");
		return -1;
	}

	int next_index_1 = -1;
	int next_index_2 = -1;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|") && next_index_1 == -1)
		{
			argv[i] = NULL;
			next_index_1 = i + 1;
		}
		else if (!strcmp(argv[i], "|") && next_index_1 != -1 && next_index_2 == -1)
		{
			argv[i] = NULL;
			next_index_2 = i + 1;
		}
	}
	if (next_index_1 == -1 || next_index_2 == -1)
	{
		printf("Error: '|' is not present\n");
		printf("Usage: ./three_pipes <command1> '|' <command2> '|' <command3>\n");
		return -1;
	}

	int pipe_fd1[2];
	int pipe_fd2[2];
	int status;
	pipe(pipe_fd1);

	int pid1 = fork();
	// int status;
	if (pid1 > 0)
	{
		// Parent

		// Create the pipe_fd2
		pipe(pipe_fd2);
		int pid2 = fork();
		if (pid2 > 0)
		{
			int pid3 = fork();
			if (pid3 > 0)
			{
				// Parent must close all pipe ends
				close(pipe_fd1[0]);
				close(pipe_fd1[1]);
				close(pipe_fd2[0]);
				close(pipe_fd2[1]);

				// Wait for all children
				waitpid(pid1, &status, 0);
				waitpid(pid2, &status, 0);
				waitpid(pid3, &status, 0);
			}
			else if (pid3 == 0)
			{
				// Child 3
				dup2(pipe_fd2[0], 0);

				close(pipe_fd1[0]);
				close(pipe_fd1[1]);
				close(pipe_fd2[1]);
				close(pipe_fd2[0]);

				execvp(argv[next_index_2], argv + next_index_2);
				perror("execvp");
			}

			// waitpid(pid3, &status, 0);
		}
		else if (pid2 == 0)
		{
			// Child 2
			close(pipe_fd1[1]);
			close(pipe_fd2[0]);

			dup2(pipe_fd1[0], 0);
			dup2(pipe_fd2[1], 1);

			close(pipe_fd1[0]);
			close(pipe_fd2[1]);

			execvp(argv[next_index_1], argv + next_index_1);
			perror("execvp");
		}
		// waitpid(pid1, &status, 0);
		// waitpid(pid2, &status, 0);
	}
	else if (pid1 == 0)
	{
		// Child 1
		close(pipe_fd1[0]);
		dup2(pipe_fd1[1], 1);
		close(pipe_fd1[1]);
		execvp(argv[1], argv + 1);
		perror("execvp");
	}
	else
	{
		perror("fork");
	}
}