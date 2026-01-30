#include "header.h"

void execute_external_commands(char *input_string)
{
    int pipe_count = 0;
    int rows = 0;

    check_rows_and_pipe(input_string, &rows, &pipe_count);
    // char *external_cmd_array[rows + 1];
    char **external_cmd_array = malloc((rows + 1) * sizeof(char *));

    extract_external_commands_from_input_string(input_string, external_cmd_array);

    if (pipe_count == 0)
    {
        execvp(external_cmd_array[0], external_cmd_array);
        perror("execvp");
        free_external_commands(external_cmd_array);
        free(external_cmd_array);
        exit(1);
    }
    else
    {
        execute_piped_commands(external_cmd_array, pipe_count);
        free_external_commands(external_cmd_array);
        free(external_cmd_array);
    }
}

void execute_piped_commands(char **external_cmd_array, int pipe_count)
{
    int pipe_fd[2];
    int cmd[pipe_count + 1];
    cmd[0] = 0;
    int count = 1;

    for (int i = 0; external_cmd_array[i] != NULL; i++)
    {
        if (strcmp(external_cmd_array[i], "|") == 0)
        {
            external_cmd_array[i] = NULL;
            cmd[count++] = i + 1;
        }
    }

    for (int i = 0; i <= pipe_count; i++)
    {
        if (i != pipe_count)
            pipe(pipe_fd);

        pid_t pid_pipe = fork();

        if (pid_pipe > 0)
        {
            if (i != pipe_count)
            {
                close(pipe_fd[1]);
                dup2(pipe_fd[0], 0);
                close(pipe_fd[0]);
            }
            wait(NULL);
        }
        else if (pid_pipe == 0)
        {
            if (i != pipe_count)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
            }

            execvp(external_cmd_array[cmd[i]], &external_cmd_array[cmd[i]]);
            perror("execvp");
            exit(1);
        }
        else
        {
            perror("fork");
        }
    }
}