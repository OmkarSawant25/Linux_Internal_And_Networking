#include <stdio.h>
#include <pthread.h>

/* Structure to store range and result for each thread */
typedef struct st
{
    int start;                     // Starting number for factorial range
    int end;                       // Ending number for factorial range
    unsigned long long fact;       // Stores partial factorial result
} values;

/* Thread function to calculate partial factorial */
void *factorial(void *arg)
{
    values *var = (values *)arg;   // Typecast void pointer to structure
    var->fact = 1;                 // Initialize factorial result

    /* Multiply numbers from start down to end */
    for (int i = var->start; i >= var->end; i--)
    {
        var->fact *= i;
    }

    return NULL;                   // No return value needed
}

int main()
{
    int n;
    printf("Enter the number : ");
    scanf("%d", &n);

    /* Handle edge cases: 0! and 1! */
    if (n == 0 || n == 1)
    {
        printf("Factorial of %d is 1\n", n);
        return 0;
    }

    pthread_t tid[3];              // Thread IDs
    values var[3];                 // Data for each thread

    int diff = n / 3;              // Divide work among 3 threads
    int prev = n;                  // Keeps track of remaining range

    /* Assign factorial ranges to each thread */
    for (int i = 0; i < 3; i++)
    {
        int start = prev;          // Start from previous value
        int end;

        /* First two threads get equal range */
        if (i != 2)
            end = prev - diff + 1;
        else
            end = 1;               // Last thread goes till 1

        prev = prev - diff;        // Update previous value

        var[i].start = start;      // Store start value
        var[i].end = end;          // Store end value
    }

    /* Create threads */
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, factorial, &var[i]);
    }

    /* Wait for all threads to finish */
    for (int i = 0; i < 3; i++)
    {
        pthread_join(tid[i], NULL);
    }

    /* Multiply partial factorial results */
    unsigned long long result =
        var[0].fact * var[1].fact * var[2].fact;

    printf("Factorial of %d is %llu\n", n, result);

    return 0;
}
