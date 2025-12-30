#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // execv needs at least program path
    if (argc < 2)
    {
        printf("Usage: %s <full_path_of_command> [args]\n", argv[0]);
        return 1;
    }

    printf("Before exec\n");

    // argv[1] -> full path of command
    // argv + 1 -> argument list for new program
    execv(argv[1], argv + 1);

    // This line executes ONLY if execv fails
    perror("execv failed");
    return 1;
}

// gcc execv.c
// ./a.out /bin/ls -l
