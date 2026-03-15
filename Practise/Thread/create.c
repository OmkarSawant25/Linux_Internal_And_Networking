#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *my_thread(void *arg)
{
    int n = 5;
    while (--n)
    {
        printf("Thread is running %d\n", n);
        sleep(1);
    }
    return NULL;
}

void *my_thread1(void *arg)
{
    int n = 5;
    while (--n)
    {
        printf("Thread 1 is running %d\n", n);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, my_thread, NULL);
    pthread_create(&t2, NULL, my_thread1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Main Finished\n");
    return 0;
}