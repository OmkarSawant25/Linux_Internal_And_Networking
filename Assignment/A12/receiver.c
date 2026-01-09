#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

void rev_string(char *str)
{
    int start = 0;
    int end = strlen(str) - 1;
    while(start <= end)
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
    // Step 1 : Get the particular SHMID
    int shmid = shmget('B', 100, 0644);

    // Attach the process with SHM
    char *shm = shmat(shmid, NULL, 0);

    rev_string(shm);
    // Print the output
    printf("Reading from shared memory :- %s\n", shm);

    // Delete the SHM
    // shmctl(shmid, IPC_RMID, NULL);
}