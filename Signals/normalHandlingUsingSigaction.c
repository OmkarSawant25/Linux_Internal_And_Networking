#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void my_handler(int signum)
{
    printf("Hello\n");
}

int main()
{
    struct sigaction new_act;
    memset(&new_act, 0, sizeof(new_act));
    new_act.sa_handler = my_handler;
    sigaction(SIGINT, &new_act, NULL);
    while(1)
        pause();
}