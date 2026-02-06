#include <stdio.h>
#include <pthread.h>

typedef struct st
{
    int start;
    int end;
} values;

void *factorial(void *arg)
{
    int fact = 1;
    values *var = (values *)arg;
    for (int i = var->start; i >= var->end; i--)
    {
        fact *= i;
    }
    return (void *)(long)fact;
}

int main()
{
    int n;
    printf("Enter the number : ");
    scanf("%d", &n);

    pthread_t tid[3];
    values var[3];
    int ans[3];

    int diff = n / 3;
    int prev = n;

    for (int i = 0; i < 3; i++)
    {
        int start = prev;
        int end;
        if (i != 2)
            end = prev - diff + 1;
        else
            end = 1;

        prev = prev - diff;
        var[i].start = start;
        var[i].end = end;
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, factorial, &var[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(tid[i], (void *)&ans[i]);
    }

    unsigned int result = ans[0] * ans[1] * ans[2];
    printf("Factorial of %d is %d\n", n, result);
}