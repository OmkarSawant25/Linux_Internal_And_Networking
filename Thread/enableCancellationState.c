#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    // Enable cancellation
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    // Deferred cancellation (default safe)
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (int i = 0; i < 10; i++)
    {
        printf("Thread running %d times\n", i + 1);
        sleep(1);   // Cancellation point
    }

    return NULL;
}

int main()
{
    pthread_t tid;

    pthread_create(&tid, NULL, fun1, NULL);

    sleep(3);

    printf("Sending cancellation request...\n");
    pthread_cancel(tid);

    pthread_join(tid, NULL);

    printf("Thread cancelled successfully\n");

    return 0;
}
