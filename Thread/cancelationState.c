#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    for (int i = 0; i < 10; sleep(1), i++)
    {
        printf("In thread %d times\n", i + 1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, fun1, NULL);
    sleep(3);
    printf("Sending cancellation request\n");
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("After pthread join\n");
    for (int i = 0; i < 10; sleep(1), i++)
    {
        printf("In main %d times\n", i + 1);
    }
}