#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
    printf("Detached thread running\n");
    sleep(2);
    printf("Detached thread finished\n");
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_DETACHED);
    pthread_create(&tid , &attr , fun , NULL);
    pthread_attr_destroy(&attr);

    sleep(3);
    printf("Main finished\n");
}