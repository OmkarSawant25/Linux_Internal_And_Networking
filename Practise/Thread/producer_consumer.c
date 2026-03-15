#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buffer;
sem_t empty, full;

void *producer(void *arg)
{
    int item = 1;

    while(1)
    {
        sem_wait(&empty);   // wait if basket full

        buffer = item;
        printf("Produced %d\n", item);

        sem_post(&full);    // tell consumer item ready

        item++;
        sleep(1);
    }
}

void *consumer(void *arg)
{
    int item;

    while(1)
    {
        sem_wait(&full);    // wait if basket empty

        item = buffer;
        printf("Consumed %d\n", item);

        sem_post(&empty);   // basket empty now

        sleep(2);
    }
}

int main()
{
    pthread_t p, c;

    sem_init(&empty, 0, 1);   // basket empty
    sem_init(&full, 0, 0);    // nothing produced

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);
}