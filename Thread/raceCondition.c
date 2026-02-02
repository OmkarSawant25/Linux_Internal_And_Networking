#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count = 0;   // Shared Data Segment Variable

void *thread_fun(void *arg)
{
    while(1)
    {
        // Critical Section (Shared Variable Access)
        count = 0;
        count = count + 5;
        count = count + 10;
        printf("Count Value = %d\n", count);
        // sleep(1);
    }
}

int main()
{
    pthread_t t1, t2;

    // Create Two Threads Using Same Function
    pthread_create(&t1, NULL, thread_fun, NULL);
    pthread_create(&t2, NULL, thread_fun, NULL);

    // Wait for Threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
