#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void *fun(void *arg)
{
    sem_wait(&sem);

    printf("Thread %ld inside CS\n", (long)arg);
    sleep(2);

    printf("Thread %ld leaving CS\n", (long)arg);

    sem_post(&sem);

    return NULL;
}

int main()
{
    pthread_t t[5];

    sem_init(&sem , 0 , 2);

    for(long i=0;i<5;i++)
        pthread_create(&t[i], NULL, fun, (void *)i);

    for(int i=0;i<5;i++)
        pthread_join(t[i], NULL);

    sem_destroy(&sem);
}