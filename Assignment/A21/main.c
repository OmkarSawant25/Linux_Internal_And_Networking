#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NO_OF_THREAD 5 // Total number of threads

typedef struct new
{
    int start; // starting index
    int end;   // ending index
    int *arr;  // pointer to array
} Block;

int g_sum = 0;           // global sum (shared by all threads)
int g_max;               // global max element
pthread_mutex_t my_lock; // mutex to protect shared data

void do_operation(Block *var)
{
    int sum = 0; // local sum

    for (int i = var->start; i <= var->end; i++)
    {
        sum += var->arr[i]; // add elements

        // check for maximum value
        if (g_max < var->arr[i])
            g_max = var->arr[i];
    }

    // add local sum to global sum
    g_sum = g_sum + sum;
}

// Thread function
void *my_fun(void *arg)
{
    Block *var = (Block *)arg;

    // lock mutex so only one thread updates g_sum and g_max
    pthread_mutex_lock(&my_lock);

    do_operation(var);

    // unlock mutex after work is done
    pthread_mutex_unlock(&my_lock);

    return NULL;
}

int main()
{
    pthread_mutex_init(&my_lock, NULL);

    pthread_t tid[NO_OF_THREAD]; // thread IDs
    Block var[NO_OF_THREAD];     // data for each thread

    int size;
    printf("Enter the size of array : ");
    scanf("%d", &size);

    int arr[size]; // array creation

    // initialize array
    for (int i = 0; i < size; i++)
    {
        arr[i] = i + 1;
    }

    g_max = arr[0]; // initialize max with first element

    // give array address to each thread structure
    for (int i = 0; i < NO_OF_THREAD; i++)
    {
        var[i].arr = arr;
    }

    int split = size / NO_OF_THREAD; // elements per thread
    int prev = 0;
    int load = size % NO_OF_THREAD; // remaining elements

    // decide start and end index for each thread
    for (int i = 0; i < NO_OF_THREAD; i++)
    {
        int start = prev;
        int end;

        if ((i == NO_OF_THREAD - 1) && load != 0)
            end = size - 1;
        else
            end = prev + split - 1;

        prev = prev + split;

        var[i].start = start;
        var[i].end = end;
    }

    // create threads
    for (int i = 0; i < NO_OF_THREAD; i++)
    {
        pthread_create(&tid[i], NULL, my_fun, &var[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < NO_OF_THREAD; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // final result
    printf("Sum is %d\n", g_sum);
    printf("Max is %d\n", g_max);

    pthread_mutex_destroy(&my_lock);
}
