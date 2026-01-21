#include <stdio.h>
#include <pthread.h>

void *fun1(void *arg)
{
    while(1)
    {
        printf("1");
    }
}

void *fun2(void *arg)
{
    while(1)
    {
        printf("2");
    }
}

int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,fun1,NULL);
    pthread_create(&tid2,NULL,fun2,NULL);
    // while(1);
}