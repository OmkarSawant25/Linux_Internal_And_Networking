#include <stdio.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        sleep(5);
        printf("Child Parent PID: %d\n", getppid());
    }
    else
    {
        printf("Parent exiting\n");
    }
}