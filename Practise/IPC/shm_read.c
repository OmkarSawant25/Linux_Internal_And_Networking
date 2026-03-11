#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    int shmid = shmget(1234, 100, 0644);
    char *ptr = shmat(shmid, NULL, 0);
    printf("Received: %s\n", ptr);
    // shmctl(shmid, IPC_RMID, NULL);
}