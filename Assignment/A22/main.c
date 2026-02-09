#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_data
{
    short m1_row;
    short m1_col;
    short m2_col;
    int **matrix1;
    int **matrix2;
    int **result;
    short cur_row;
} Thread_data_t;

// Thread function
void *my_fun(void *arg)
{
    Thread_data_t *var = (Thread_data_t *)arg;

    for (int j = 0; j < var->m2_col; j++)
    {
        int sum = 0;
        for (int k = 0; k < var->m1_col; k++)
        {
            sum += var->matrix1[var->cur_row][k] * var->matrix2[k][j];
        }
        var->result[var->cur_row][j] = sum;
    }
}

int main()
{
    int m, n, p, q;
    printf("Enter M1 rows and columns : ");
    scanf("%d%d", &m, &n);

    printf("Enter M2 rows and columns : ");
    scanf("%d%d", &p, &q);

    if (n != p)
    {
        printf("The matrices cannot be multiplied\n");
    }
    else
    {
        // Allocate array of row pointer
        int **M1 = (int **)malloc(sizeof(int *) * m);

        // Allocate each row individually
        for (int i = 0; i < m; i++)
        {
            M1[i] = (int *)malloc(sizeof(int) * n);
        }

        // Allocate array of row pointer
        int **M2 = (int **)malloc(sizeof(int *) * p);

        // Allocate each row individually
        for (int i = 0; i < p; i++)
        {
            M2[i] = (int *)malloc(sizeof(int) * q);
        }

        // Allocate array of row pointer
        int **result = (int **)malloc(sizeof(int *) * m);

        // Allocate each row individually
        for (int i = 0; i < m; i++)
        {
            result[i] = (int *)malloc(sizeof(int) * q);
        }

        printf("Enter M1 rows and columns\n");
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                scanf("%d", &M1[i][j]);
            }
        }

        printf("Enter M2 rows and columns\n");
        for (int i = 0; i < p; i++)
        {
            for (int j = 0; j < q; j++)
            {
                scanf("%d", &M2[i][j]);
            }
        }

        pthread_t tid[m];
        Thread_data_t var[m];
        int col = 0;

        for (int i = 0; i < m; i++)
        {
            var[i].cur_row = i;
            var[i].m1_col = n;
            var[i].m2_col = q;
            var[i].matrix1 = M1;
            var[i].matrix2 = M2;
            var[i].result = result;
            pthread_create(&tid[i], NULL, my_fun, &var[i]);
        }
        for (int i = 0; i < m; i++)
        {
            pthread_join(tid[i], NULL);
        }
        printf("Result is : \n");
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < q; j++)
            {
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
    }
}