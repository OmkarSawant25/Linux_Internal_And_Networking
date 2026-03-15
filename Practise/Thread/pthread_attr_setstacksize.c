#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
    int arr[10000];   // large local array -> needs stack
    printf("Thread running with custom stack\n");
    sleep(2);
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr , 1024 * 1024); // 1 MB stack
    pthread_create(&tid , &attr , fun , NULL);
    pthread_join(tid , NULL);
    pthread_attr_destroy(&attr);

    printf("Main finished\n");
}