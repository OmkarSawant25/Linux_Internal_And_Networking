#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_mutexattr_t attr;

void *thread(void *arg)
{
    pthread_mutex_lock(&lock);
    printf("Locked first time\n");

    pthread_mutex_lock(&lock);
    printf("Locked second time (Allowed)\n");

    pthread_mutex_unlock(&lock);
    printf("Unlocked once\n");

    pthread_mutex_unlock(&lock);
    printf("Unlocked twice\n");

    return NULL;
}

int main()
{
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

    pthread_mutex_init(&lock, &attr);

    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}
