#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    int shmid = shmget(1234,100,IPC_CREAT | 0644);
    char *ptr = shmat(shmid, NULL, 0);
    strcpy(ptr, "Hello Shared Memory Omkar");
    printf("Data Written\n");
    shmdt(ptr);
}