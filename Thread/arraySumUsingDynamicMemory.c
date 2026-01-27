#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct input
{
    int st;
    int end;
    int *ptr;
};

void *array_sum(void *arg)
{
    int *sum = malloc(sizeof(int));
    *sum = 0;

    struct input *str = arg;

    for (int i = str->st; i <= str->end; i++)
    {
        *sum += str->ptr[i];
    }
    return sum;
}

int main()
{
    pthread_t tid;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int *sum;

    struct input var;
    var.st = 0;
    var.end = 9;
    var.ptr = arr;

    pthread_create(&tid, NULL, array_sum, &var);
    pthread_join(tid, (void *)&sum); // waiting for the thread to get terminated
    printf("The sum of array is -> %d\n", *sum);
    free(sum); // free heap memory
}