#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // getpid()
#include <sys/types.h>
#include <sys/wait.h> // wait()

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage : ./a.out filename\n");
        return 0;
    }

    close(1);
    // Open the file
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    struct flock var;

    // Create the process
    int pid = fork();

    if (pid > 0)
    {
        // Parent Process
        var.l_type = F_WRLCK;
        var.l_whence = SEEK_SET;
        var.l_start = 0;
        var.l_len = 0;
        var.l_pid = getpid();

        fcntl(fd, F_SETLKW, &var);

        // Print Prime Number
        for (int i = 2; i <= 15; i++)
        {
            int count = 0;
            for (int j = 2; j <= i / 2; j++)
            {
                if ((i % j) == 0)
                {
                    count++;
                }
            }
            if (count == 0)
            {
                printf("%d ", i);
            }
        }
        printf("\n");

        var.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &var);
        wait(NULL);
    }
    else if (pid == 0)
    {
        // Child Process
        var.l_type = F_WRLCK;
        var.l_whence = SEEK_SET;
        var.l_start = 0;
        var.l_len = 0;
        var.l_pid = getpid();

        fcntl(fd, F_SETLKW, &var);

        // Print Fibonacci Series

        int fibo1 = 0, fibo2 = 1, sum = 0, count = 0;

        while (count <= 10)
        {
            printf("%d ", fibo1);
            sum = fibo1 + fibo2;
            fibo1 = fibo2;
            fibo2 = sum;
            count++;
        }
        printf("\n");
        var.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &var);
    }
    else
    {
        perror("Fork");
    }
    return 0;
}