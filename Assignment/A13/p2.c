#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Function to reverse the string
void reverse(char *str)
{
    int start = 0;
    int end = strlen(str) - 1;

    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int main()
{
    // Get shared memory created by process 1
    int shmid = shmget('Z', 100, 0644);

    // Attach shared memory
    char *shm = shmat(shmid, NULL, 0);

    char str[100];

    // Copy data from shared memory
    strcpy(str, shm);

    // Detach shared memory
    shmdt(shm);

    // Remove shared memory
    shmctl(shmid, IPC_RMID, NULL);

    // Reverse the string
    reverse(str);

    // Create FIFO for communication
    mkfifo("/tmp/my_own_fifo", 0644);

    // Open FIFO in write mode
    int fd = open("/tmp/my_own_fifo", O_WRONLY);

    // Write reversed string to FIFO
    write(fd, str, strlen(str) + 1);

    // Close FIFO
    close(fd);

    return 0;
}