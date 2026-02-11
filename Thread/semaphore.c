#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NO_OF_THREADS 5

// Step 1
sem_t my_count;
int no;

void *thread_function(void *arg)
{
    sem_wait(&my_count); // Step 3
    int n = ++no;
    for (int i = 0; i < 2; sleep(1), i++)
    {
        printf("Thread %d running %d times \n", n, i + 1);
    }
    sem_post(&my_count); // Step 4
    return NULL;
}

int main()
{
    pthread_t tid[NO_OF_THREADS];

    // Step 2
    sem_init(&my_count, 0, 2);

    // Create Threads
    for (int i = 0; i < NO_OF_THREADS; i++)
    {
        pthread_create(&tid[i], NULL, thread_function, NULL);
    }

    // Join Threads
    for (int i = 0; i < NO_OF_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Step 5
    sem_destroy(&my_count);

    return 0;
}
