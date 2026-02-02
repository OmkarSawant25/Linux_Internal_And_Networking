#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_mutexattr_t attr;

void *thread(void *arg)
{
    pthread_mutex_lock(&lock);
    printf("Locked first time\n");

    // Second lock gives error, no deadlock
    int ret = pthread_mutex_lock(&lock);

    if (ret != 0)
        printf("Error: Mutex already locked by same thread!\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);

    pthread_mutex_init(&lock, &attr);

    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}
