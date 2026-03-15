#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        printf("Running fast\n");
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, fun, NULL);
    sleep(1);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("Thread killed instantly\n");
}