#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int shmid;
    struct shmid_ds shm_info;

    // Get shared memory ID
    shmid = shmget('A', 100, 0644);
    if (shmid == -1)
    {
        perror("shmget failed");
        return 1;
    }

    // Get shared memory status
    if (shmctl(shmid, IPC_STAT, &shm_info) == -1)
    {
        perror("shmctl IPC_STAT failed");
        return 1;
    }

    // Print shared memory status
    printf("Shared Memory Status:\n");
    printf("SHM ID          : %d\n", shmid);
    printf("Size            : %lu bytes\n", shm_info.shm_segsz);
    printf("Owner UID       : %d\n", shm_info.shm_perm.uid);
    printf("Owner GID       : %d\n", shm_info.shm_perm.gid);
    printf("Creator UID     : %d\n", shm_info.shm_perm.cuid);
    printf("Creator GID     : %d\n", shm_info.shm_perm.cgid);
    printf("Permissions     : %o\n", shm_info.shm_perm.mode);
    printf("No. of attaches : %lu\n", shm_info.shm_nattch);

    return 0;
}
