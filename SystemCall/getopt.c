#include <stdio.h>
#include <unistd.h> // getopt
#include <fcntl.h>

extern int optind;

int main(int argc, char *argv[])
{
    printf("Before getopt : ");
    for (int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    int ret;
    while ((ret = getopt(argc, argv, "lwc")) != -1)
    {
        printf("Opt -> %c\n", ret);
    }

    printf("After getopt : ");
    for (int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    printf("Pos is %d\n", optind);

    return 0;
}
