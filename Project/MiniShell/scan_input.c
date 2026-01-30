#include "header.h"

char *external_commands[200];
pid_t pid;
int status;
int last_exit_status;
stopped_jobs *head = NULL;

void scan_input(char *prompt, char *input_string)
{
    pid = 0;
    free_external_commands(external_commands);
    extract_external_commands(external_commands);

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    // signal(SIGCHLD, signal_handler);

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

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT)
    {
        if (pid == 0)
        {
            printf("\n%s ", prompt);
            fflush(stdout);
        }
    }
    if (sig_num == SIGTSTP)
    {
        if (pid == 0)
        {
            printf("\n%s ", prompt);
            fflush(stdout);
        }
        else if (pid > 0)
        {
            insert_jobs_at_start(&head);
        }
    }
    else if (sig_num == SIGCHLD)
    {
        waitpid(-1, &status, WNOHANG);
    }
}
