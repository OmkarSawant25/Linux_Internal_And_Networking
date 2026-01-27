#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum;

struct input
{
	int st;
	int end;
	int *ptr;
};

void array_sum(void *arg)
{
	struct input *str = arg;
	for (int i = str->st; i <= str->end; i++)
	{
		sum = sum + str->ptr[i];
	}
}

int main()
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	struct input var;
	var.st = 0;
	var.end = 9;
	var.ptr = arr;
	pthread_t tid;
	pthread_create(&tid, NULL, (void *)array_sum, &var);
	pthread_join(tid, NULL);
	printf("The sum of the array is -> %d\n", sum);
}
