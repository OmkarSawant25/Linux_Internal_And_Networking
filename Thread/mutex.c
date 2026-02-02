#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count;
pthread_mutex_t my_lock; // STEP 1

void *thread(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&my_lock); // STEP 3
        count = 0;
        count += 5;
        count += 10;
        printf("count is %d\n", count);
        sleep(1);
        pthread_mutex_unlock(&my_lock); // STEP 4
    }
}

int main()
{
    pthread_t tid1, tid2;
    pthread_mutex_init(&my_lock, NULL); // STEP 2

    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&my_lock); // STEP 5
}