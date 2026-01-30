#include "header.h"

char prompt[25] = "MiniShell$";

int main()
{
    char input_string[1024];
    scan_input(prompt, input_string);
    return 0;
}
