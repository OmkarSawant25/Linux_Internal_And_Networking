#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

char *external_commands[153];
int pid;
int status;
extern char prompt[25];
int last_exit_status;

void scan_input(char *prompt, char *input_string)
{
    free_external_commands(external_commands);
    extract_external_commands(external_commands);

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    while (1)
    {
        printf("%s ", prompt);
        scanf("%[^\n]", input_string);
        getchar();

        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            int i = 4;
            int flag = 1;

            remove_trailing_space(input_string);

            while (input_string[i] != '\0')
            {
                if (input_string[i] == ' ')
                {
                    flag = 0;
                    break;
                }
                i++;
            }

            if (flag)
                strcpy(prompt, input_string + 4);
            else
                printf("Error: Space\n");
        }
        else
        {
            char *cmd = get_command(input_string);
            int type = check_command_type(cmd);

            if (type == BUILTIN)
            {
                execute_internal_commands(input_string);
                last_exit_status = 0;
            }
            else if (type == EXTERNAL)
            {
                pid = fork();
                if (pid > 0)
                {
                    // Parent
                    waitpid(pid, &status, WUNTRACED);
                    if (WIFEXITED(status))
                    {
                        last_exit_status = WEXITSTATUS(status);
                    }
                    else if (WIFSTOPPED(status))
                    {
                        last_exit_status = 128 + WSTOPSIG(status);
                    }

                    pid = 0;
                }
                else if (pid == 0)
                {
                    // Child
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands(input_string);
                    exit(0);
                }
                else
                {
                    perror("fork");
                }
            }
            else if (type == NO_COMMAND)
            {
                printf("Command not found: %s\n", cmd);
            }
            else
            {
                perror("fork");
            }
        }
    }
}

void remove_trailing_space(char *input_string)
{
    int len = strlen(input_string) - 1;
    while (input_string[len] == ' ' && (len >= 0))
    {
        input_string[len--] = '\0';
    }
}

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT || sig_num == SIGTSTP)
    {
        if (pid <= 0)
        {
            printf("\n%s ", prompt);
            fflush(stdout);
        }
    }
}

char *get_command(char *input_string)
{
    static char cmd[50];
    int i = 0;

    while (input_string[i] != ' ' && input_string[i] != '\0')
    {
        cmd[i] = input_string[i];
        i++;
    }

    cmd[i] = '\0';
    return cmd;
}

int check_command_type(char *command)
{
    if (check_internal_command(command))
        return BUILTIN;
    else if (check_external_command(command))
        return EXTERNAL;
    else
        return NO_COMMAND;
}

bool check_internal_command(char *command)
{
    int i = 0;
    while (builtins[i] != NULL)
    {
        if (strcmp(builtins[i], command) == 0)
            return true;
        i++;
    }
    return false;
}

bool check_external_command(char *command)
{
    int i = 0;
    while (external_commands[i] != NULL)
    {
        if (strcmp(external_commands[i], command) == 0)
            return true;
        i++;
    }
    return false;
}

void extract_external_commands(char **external_commands)
{
    int fd = open("ext_cmd.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    char ch;
    char buff[256];
    int row = 0, col = 0;
    while (read(fd, &ch, 1) == 1)
    {
        if (ch == '\n')
        {
            // buff[col] = '\0';
            // external_commands[row] = malloc(strlen(buff) + 1);
            // strcpy(external_commands[row], buff);
            // row++;
            allocate_dynamic_cols(external_commands, buff, &col, &row);
            col = 0;
        }
        else
        {
            buff[col++] = ch;
        }
    }

    // buff[col] = '\0';
    // external_commands[row] = malloc(strlen(buff) + 1);
    // strcpy(external_commands[row], buff);
    // row++;
    allocate_dynamic_cols(external_commands, buff, &col, &row);

    external_commands[row] = NULL;
    close(fd);
}

void allocate_dynamic_cols(char **external_commands, char *buff, int *col, int *row)
{
    buff[*col] = '\0';
    external_commands[*row] = malloc(strlen(buff) + 1);
    if (external_commands[*row] == NULL)
    {
        perror("malloc");
        return;
    }
    strcpy(external_commands[*row], buff);
    (*row)++;
}

void execute_internal_commands(char *input_string)
{
    char buff[100];

    if (strcmp(input_string, "exit") == 0)
    {
        free_external_commands(external_commands);
        exit(0);
    }

    else if (strcmp(input_string, "pwd") == 0)
    {
        if (getcwd(buff, sizeof(buff)) != NULL)
            printf("%s\n", buff);
        else
            perror("pwd");
    }
    else if (strncmp(input_string, "cd", 2) == 0)
    {
        if (input_string[2] == ' ')
        {
            if (chdir(input_string + 3) != 0)
                perror("cd");
        }
        else
        {
            char *home = getenv("HOME");
            if (home)
            {
                if (chdir(home) != 0)
                    perror("cd");
            }
            else
            {
                printf("cd: HOME not set\n");
            }
        }
    }
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("%d\n", getpid());
    }
    else if (strcmp(input_string, "echo $?") == 0)
    {
        printf("%d\n", last_exit_status);
    }
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        char *ptr = getenv("SHELL");
        if (ptr)
            printf("%s\n", ptr);
        else
            printf("SHELL not set\n");
    }
}

void free_external_commands(char **external_commands)
{
    int i = 0;
    while (external_commands[i] != NULL)
    {
        free(external_commands[i]);
        i++;
    }
}

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

        int pid_pipe = fork();

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

void extract_external_commands_from_input_string(char *input_string, char *external_cmd_array[])
{
    int row = 0;
    int col = 0;
    char buff[100];

    for (int i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == ' ')
        {
            if (col > 0)
            {
                allocate_dynamic_cols(external_cmd_array, buff, &col, &row);
                col = 0;
            }
        }
        else
        {
            buff[col++] = input_string[i];
        }
    }

    // last token
    if (col > 0)
    {
        allocate_dynamic_cols(external_cmd_array, buff, &col, &row);
    }

    external_cmd_array[row] = NULL;
}

void check_rows_and_pipe(char *input_string, int *row, int *pipe_count)
{
    int i;
    *row = 0;
    *pipe_count = 0;

    for (i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == '|')
            (*pipe_count)++;

        if (input_string[i] == ' ')
            (*row)++;
    }

    (*row)++;
}
