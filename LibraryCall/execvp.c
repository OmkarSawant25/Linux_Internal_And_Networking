// ./a.out ls -l

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s command [args]\n", argv[0]);
        return 1;
    }

    printf("Before exec\n");

    execvp(argv[1], argv + 1);

    // Runs only if execvp fails
    perror("execvp failed");
    return 1;
}
