#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void my_handler(int sig)
{
    printf("\nSIGINT received\n");
}

int main()
{
    signal(SIGINT, my_handler);

    while(1)
    {
        printf("Running...\n");
        sleep(1);
    }
}