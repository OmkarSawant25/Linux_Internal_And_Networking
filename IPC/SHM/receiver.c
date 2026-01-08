#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

int main()
{
    // Step 1 : Get the particular SHMID
    int shmid = shmget('A', 100, 0644);

    // Attach the process with SHM
    char *ptr = shmat(shmid, NULL, 0);

    // Print the output
    printf("The data from SHM is -> %s\n", ptr);

    // Delete the SHM
    shmctl(shmid, IPC_RMID, NULL);
}