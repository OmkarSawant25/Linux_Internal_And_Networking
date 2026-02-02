/*
===============================================================================
Project Title : MiniShell – Custom Command Line Interpreter
Name          : Omkar Ashok Sawant
Batch Id      : 25021C_309
Date          : 02/02/2026
Language      : C Programming
===============================================================================

Project Overview:
    MiniShell is a custom Linux command-line interpreter developed in C that
    replicates core functionalities of a Unix shell. It provides an interactive
    prompt to execute internal and external commands while managing processes
    and signals efficiently.

Features:
    1. Command Execution:
       - Executes external Linux commands using fork(), execvp(), and waitpid().

    2. Internal Commands:
       - Implements built-in commands such as cd, pwd, exit, jobs, fg, and bg.

    3. Process Management:
       - Supports foreground and background execution.
       - Maintains job control for stopped and running processes.

    4. Signal Handling:
       - Handles SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z) without terminating shell.

    5. Prompt Handling:
       - Displays a customizable shell prompt (PS1 support).
       - Continuously accepts user input.

    6. Command Validation:
       - Identifies valid external commands using an external command list file.

File Structure:
    - main.c           : Initializes shell and controls execution flow.
    - scan_input.c     : Reads user input and parses commands.
    - internal_cmd.c   : Handles built-in shell commands.
    - external_cmd.c   : Executes external Linux commands.
    - commands.c       : Command classification and dispatch logic.
    - header.h         : Common macros, definitions, and function prototypes.
    - ext_cmd.txt      : List of supported external commands.
    - a.out            : Compiled executable output.

Learning Outcome:
    - In-depth understanding of Linux system calls, process control,
      signal handling, and command-line interpreter design.

===============================================================================
*/

#include "header.h"

// Initializing the shell prompt string
char prompt[MAX_PROMPT_LEN] = "MiniShell$";

// Buffer to store user input command
char input_string[MAX_INPUT_LEN];

int main()
{
    // Clearing the terminal screen before starting the shell
    system("clear");

    // Calling the scan_input function to start shell execution
    // This function continuously displays the prompt and processes user commands
    scan_input(prompt, input_string);

    // Returning control to the operating system
    return 0;
}
