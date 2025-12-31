#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int pipe_fd[2];
    int pid;

    pipe(pipe_fd);
    pid = fork();

    if (pid > 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        close(pipe_fd[1]);
        execl("/bin/ls", "ls", NULL);
    }
    else
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        close(pipe_fd[0]);
        execl("/usr/bin/wc", "wc", NULL);
    }

    return 0;
}
