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