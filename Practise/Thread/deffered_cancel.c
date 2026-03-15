#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
    while (1)
    {
        printf("Working...\n");
        sleep(2); // cancellation point
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, fun, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("Thread terminated safely\n");
}