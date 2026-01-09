#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <ctype.h>
#include <unistd.h>

int main()
{
    // Step 1 : Create the SHM & get the shmid
    int shmid = shmget('B', 100, IPC_CREAT | 0644);

    // Step 2 : Attach the current process with SHM
    char *shm = shmat(shmid, NULL, 0);

    // Step 3 : Read string from user
    char str[100];
    printf("Enter the string\n");
    scanf("%s", str);

    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }

    // Step 4 : Copy the readed string into SHM
    strcpy(shm, str);

    printf("Writing to shared memory ...\n");

    sleep(10);

    printf("Reading from shared memory : %s\n", shm);

    // Step 5 : Dettach the SHM
    shmdt(shm);
}