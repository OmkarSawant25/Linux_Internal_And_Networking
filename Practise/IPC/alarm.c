#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    printf("Alarm Signal Received\n");
}

int main()
{
    signal(SIGALRM, handler);

    alarm(3);

    pause();

    return 0;
}