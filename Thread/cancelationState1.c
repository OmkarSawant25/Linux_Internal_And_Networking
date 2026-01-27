#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    // Asynchronous cancellation means:
    // Thread will be cancelled immediately,
    // not waiting for cancellation points like sleep().
    // So thread can stop at any moment.

    for (int i = 0; i < 10; sleep(1), i++)
    {
        printf("In thread %d times\n", i + 1);
    }
    
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, fun1, NULL);
    sleep(3);
    printf("Sending cancellation request\n");
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("After pthread join\n");
    for (int i = 0; i < 10; sleep(1), i++)
    {
        printf("In main %d times\n", i + 1);
    }
}