#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_mutexattr_t attr;

void *thread(void *arg)
{
    pthread_mutex_lock(&lock);
    printf("Locked first time\n");

    // Lock again (Deadlock)
    pthread_mutex_lock(&lock);
    printf("Locked second time\n"); // Never reached

    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, NULL);

    pthread_mutex_init(&lock, &attr);

    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}


// Locked first time
// (Hangs forever due to deadlock)
