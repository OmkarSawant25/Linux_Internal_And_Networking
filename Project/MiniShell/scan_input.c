#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

char *external_commands[153];
void scan_input(char *prompt, char *input_string)
{
    extract_external_commands(external_commands);

    while (1)
    {
        printf("%s ", prompt);
        scanf("%[^\n]", input_string);
        getchar();

        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            int i = 4;
            int flag = 1;

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
            }
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
            buff[col] = '\0';
            external_commands[row] = malloc(strlen(buff) + 1);
            strcpy(external_commands[row], buff);

            col = 0;
            row++;
        }
        else
        {
            buff[col++] = ch;
        }
    }

    buff[col] = '\0';
    external_commands[row] = malloc(strlen(buff) + 1);
    strcpy(external_commands[row], buff);
    row++;

    external_commands[row] = NULL;
    close(fd);
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
            // cd without path -> do nothing
            printf("cd: missing operand\n");
        }
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
