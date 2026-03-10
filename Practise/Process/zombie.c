#include <stdio.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        printf("Child exiting\n");
    }
    else
    {
        sleep(20);
    }
}