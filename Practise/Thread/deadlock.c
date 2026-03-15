#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m1, m2;

void *t1_fun(void *arg)
{
    pthread_mutex_lock(&m1);
    printf("Thread1 locked m1\n");

    sleep(1);

    pthread_mutex_lock(&m2);
    printf("Thread1 locked m2\n");

    pthread_mutex_unlock(&m2);
    pthread_mutex_unlock(&m1);

    return NULL;
}

void *t2_fun(void *arg)
{
    pthread_mutex_lock(&m2);
    printf("Thread2 locked m2\n");

    sleep(1);

    pthread_mutex_lock(&m1);
    printf("Thread2 locked m1\n");

    pthread_mutex_unlock(&m1);
    pthread_mutex_unlock(&m2);

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);

    pthread_create(&t1, NULL, t1_fun, NULL);
    pthread_create(&t2, NULL, t2_fun, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

// ⭐ What Happens Here ?

// Execution:

// Thread1 locks m1
// Thread2 locks m2

// Thread1 waits for m2
// Thread2 waits for m1

// Now:

// 👉 Both waiting forever
// 👉 Program hangs

// This is DEADLOCK.