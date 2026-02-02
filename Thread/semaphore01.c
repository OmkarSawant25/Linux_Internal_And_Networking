#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NO_OF_THREADS 5

int no;
sem_t my_lock; // step 1

void *thread_function(void *args)
{
    int n = ++no;
    sem_wait(&my_lock); // step 3
    for (int i = 0; i < 2; sleep(1), i++)
    {
        printf("Thread %d running %d times\n", n, i + 1);
    }
    sem_post(&my_lock); // step 4
}

int main()
{
    pthread_t tid[NO_OF_THREADS];

    sem_init(&my_lock, 0, 2); // step 2
    // Create Thread
    for (int i = 0; i < NO_OF_THREADS; i++)
    {
        pthread_create(&tid[i], NULL, thread_function, NULL);
    }
    // Join Thread
    for (int i = 0; i < NO_OF_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&my_lock); // step 5
}