#include "header.h"

extern int last_exit_status;
extern char *external_commands[200];
extern stopped_jobs *head;

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
    else if (strcmp(input_string, "jobs") == 0)
    {
        print_jobs(head);
    }
    else if (strcmp(input_string, "fg") == 0)
    {
        if (head != NULL)
        {
            kill(head->pid, SIGCONT);
            pid = head->pid;
            printf("%s\n", head->sleep_command);
            waitpid(pid, &status, WUNTRACED);
            pid = 0;
            delete_jobs(&head);
        }
    }
    else if (strcmp(input_string, "bg") == 0)
    {
        if (head != NULL)
        {
            signal(SIGCHLD, signal_handler);
            kill(head->pid, SIGCONT);
            printf("[%d] %s &\n", head->pid, head->sleep_command);
            delete_jobs(&head);
        }
        else
        {
            printf("bg: no current job\n");
        }
    }
}