/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : Process State Observation using /proc

   Description :
   This program demonstrates:
   1. Creation of a child process using fork()
   2. Reading process information from /proc/<pid>/status
   3. Observing process state before and after sleep
   4. Creation of an orphan process and adoption by init/systemd

   Problem Statement :
   Write a C program to create a child process and read its
   status from the /proc filesystem. Display the first three
   lines of the status file before and after sleep.
   Then create another child process to observe parent
   termination and init adoption behavior.

   OUTPUT :
   A child created with pid 15185
   Name:   a.out
   Umask:  0022
   State:  S (sleeping)
   Name:   a.out
   State:  Z (zombie)
   Tgid:   15185
   Process 15185 Cleared by init
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int pid = fork(); // Create first child process
    char buff[50];    // To store /proc/<pid>/status path
    int count = 0;
    char ch;

    // Parent process
    if (pid > 0)
    {
        printf("A child created with pid %d\n", pid);
        sprintf(buff, "/proc/%d/status", pid);

        int fd = open(buff, O_RDONLY); // Open the status file of child process

        // Read and print first 3 lines of status file
        while (count < 3)
        {
            read(fd, &ch, 1);
            if (ch == '\n')
                count++;
            printf("%c", ch);
        }

        sleep(10);

        // Reset file offset
        lseek(fd, 0, SEEK_SET);
        count = 0;

        // Read and print first 3 lines of status file
        while (count < 3)
        {
            read(fd, &ch, 1);
            if (ch == '\n')
                count++;
            printf("%c", ch);
        }

        // Create second child to demonstrate orphan process
        int pid2 = fork();
        if (pid2 > 0)
        {
            sleep(5); // Parent sleeps and exits
        }
        else if (pid2 == 0)
        {
            int old = getppid();
            sleep(10);
            int new = getppid();
            if (old != new)
            {
                printf("Process %d Cleared by init\n", pid);
            }
        }
    }
    // Child process
    else if (pid == 0)
    {
        sleep(5);
    }

    return 0;
}
