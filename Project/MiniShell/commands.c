#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg", "bg", NULL};

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
            allocate_dynamic_cols(external_commands, buff, &col, &row);
            col = 0;
        }
        else
        {
            buff[col++] = ch;
        }
    }
    allocate_dynamic_cols(external_commands, buff, &col, &row);

    external_commands[row] = NULL;
    close(fd);
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

void remove_trailing_space(char *input_string)
{
    int len = strlen(input_string) - 1;
    while (input_string[len] == ' ' && (len >= 0))
    {
        input_string[len--] = '\0';
    }
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

void print_jobs(stopped_jobs *head)
{
    int i = 1;
    while(head)
    {
        printf("[%d]   Stopped                 %s\n", i++, head->sleep_command);
        head = head->next;  
    }
}

void insert_jobs_at_start(stopped_jobs **head)
{
    stopped_jobs *newNode = (stopped_jobs *)malloc(sizeof(stopped_jobs));

    if(newNode == NULL)
        return;
    
    newNode->pid = pid;
    strcpy(newNode->sleep_command,input_string);
    newNode->next = NULL;

    if(*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        newNode->next = *head;
        *head = newNode;
    }
}

void delete_jobs(stopped_jobs **head)
{
    stopped_jobs *temp = *head;
    *head = temp->next;
    free(temp);
}