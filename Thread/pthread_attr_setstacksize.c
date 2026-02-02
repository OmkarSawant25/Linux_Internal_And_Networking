#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *fun1(void *arg)
{
    printf("Thread running successfully...\n");
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1;
    pthread_attr_t my_attr;
    size_t size;

    // Step 1: Initialize attribute
    pthread_attr_init(&my_attr);

    // Step 2: Get default stack size
    pthread_attr_getstacksize(&my_attr, &size);
    printf("Default Stack Size = %lu bytes\n", size);

    // Step 3: Set new stack size (minimum 16384 bytes)
    pthread_attr_setstacksize(&my_attr, 200000);

    // Step 4: Get updated stack size
    pthread_attr_getstacksize(&my_attr, &size);
    printf("Updated Stack Size = %lu bytes\n", size);

    // Step 5: Create thread with new stack size
    pthread_create(&tid1, &my_attr, fun1, NULL);

    pthread_join(tid1, NULL);

    // Step 6: Destroy attribute object
    pthread_attr_destroy(&my_attr);

    return 0;
}
