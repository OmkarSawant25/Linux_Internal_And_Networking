#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    alarm(10);
    for(int i = 0; i < 7;sleep(1),i++)
    {
        printf("Hi\n");
    }
    alarm(15);
    pause();
}