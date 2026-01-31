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
