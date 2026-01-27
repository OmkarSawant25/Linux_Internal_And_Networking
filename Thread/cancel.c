#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *fun1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    for (int i = 0; i < 10; sleep(1), i++)
    {
        printf("Inside thread %d times\n", i + 1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,fun1,NULL);
    
}