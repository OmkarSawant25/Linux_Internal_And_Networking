#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    printf("Signal received\n");
}

int main()
{
    signal(SIGINT, handler);

    printf("Waiting for signal...\n");

    pause();

    printf("Pause finished\n");

    return 0;
}