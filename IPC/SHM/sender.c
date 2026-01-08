#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    // Step 1 : Create the SHM & get the shmid
    int shmid = shmget('A', 100, IPC_CREAT | 0644);

    // Step 2 : Attach the current process with SHM
    char *shm = shmat(shmid, NULL, 0);

    // Step 3 : Read string from user
    char str[50];
    printf("Enter the string\n");
    scanf("%s", str);

    // Step 4 : Copy the readed string into SHM
    strcpy(shm, str);

    // Step 5 : Dettach the SHM
    shmdt(shm);
    return 0;
}