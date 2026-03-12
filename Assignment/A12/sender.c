/*
   Name        : Omkar Ashok Sawant
   Program     : A12 – Implement communication between two different processes using Shared Memory

   Description :
   This program demonstrates inter-process communication (IPC)
   between two different processes using shared memory (SHM).
   Two separate executable programs are run in different terminals.
   One process creates the shared memory and writes data into it,
   while the other process reads and modifies the same data.

   In Process 1, the user is prompted to enter a string which is
   converted into uppercase and written into shared memory.
   Process 2 reads the data from shared memory, reverses the string
   and writes the modified string back into shared memory.
   Finally, Process 1 reads the updated string and prints the
   result on standard output.

   The program demonstrates:
   1. Creation and attachment of shared memory using shmget() and shmat()
   2. Communication between unrelated processes using shared memory
   3. Reading and writing data from shared memory
   4. String manipulation like uppercase conversion and reversing
   5. Synchronization concept between two different processes

   Problem Statement :
   Write two separate C programs to implement communication between
   two different processes using shared memory. One process should
   take user input, convert it to uppercase and write to shared memory.
   The second process should read the data, reverse the string and
   write it back. The first process should then read and print the
   final output.
*/

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