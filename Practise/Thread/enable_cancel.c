#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_fun(void *arg)
{
    while (1)
    {
        printf("Thread running...\n");
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;

    pthread_create(&tid, NULL, thread_fun, NULL);

    sleep(3);

    pthread_cancel(tid);

    pthread_join(tid, NULL);

    printf("Thread cancelled\n");
}