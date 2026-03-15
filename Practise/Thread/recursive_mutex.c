#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

int main()
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_settype(&attr , PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&lock , &attr);

    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);

    printf("Recursive lock success\n");

    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);

    pthread_mutex_destroy(&lock);
}