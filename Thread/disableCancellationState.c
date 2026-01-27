#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    // Disable cancellation
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    for (int i = 0; i < 10; i++)
    {
        printf("Thread running %d times\n", i + 1);
        sleep(1);
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

    printf("Thread completed normally (not cancelled)\n");

    return 0;
}
