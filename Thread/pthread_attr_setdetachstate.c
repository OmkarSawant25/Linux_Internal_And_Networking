#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *fun1(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        printf("In thread: %d times\n", i);
        sleep(1);
    }

    pthread_exit("Thread Completed");
}

int main()
{
    pthread_t tid1;
    pthread_attr_t my_attr;
    void *res;

    // Step 1: Initialize attribute
    pthread_attr_init(&my_attr);

    // Step 2: Set thread as Detached
    pthread_attr_setdetachstate(&my_attr, PTHREAD_CREATE_DETACHED);

    // Step 3: Create thread
    pthread_create(&tid1, &my_attr, fun1, NULL);

    printf("Thread created in Detached state...\n");

    // Step 4: pthread_join will NOT work for detached thread
    int ret = pthread_join(tid1, &res);

    if (ret != 0)
        printf("pthread_join failed because thread is Detached!\n");

    // Step 5: Destroy attribute object
    pthread_attr_destroy(&my_attr);

    sleep(6); // wait so detached thread can finish
    printf("Main thread exiting...\n");

    return 0;
}
