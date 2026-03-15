#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    while (1)
    {
        printf("Cannot cancel me\n");
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, fun, NULL);
    sleep(3);
    pthread_cancel(tid);
    printf("Cancel request sent\n");
    pthread_join(tid, NULL);
}