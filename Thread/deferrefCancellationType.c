#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    // Set cancellation type to DEFERRED (Default)
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (int i = 0; i < 10; i++)
    {
        printf("In thread %d times\n", i + 1);

        sleep(1);   // Cancellation Point
    }

    return NULL;
}

int main()
{
    pthread_t tid;

    // Create thread
    pthread_create(&tid, NULL, fun1, NULL);

    // Main sleeps for 3 seconds
    sleep(3);

    // Send cancellation request
    printf("Sending cancellation request\n");
    pthread_cancel(tid);

    // Wait for thread termination
    pthread_join(tid, NULL);

    printf("After pthread join\n");

    // Main loop
    for (int i = 0; i < 10; i++)
    {
        printf("In main %d times\n", i + 1);
        sleep(1);
    }

    return 0;
}
