/*
   Name        : Omkar Ashok Sawant
   Program     : A13 – Implement communication between three processes using PIPE, FIFO and SHM

   Description :
   This program demonstrates inter-process communication (IPC)
   between three different processes using three IPC mechanisms:
   Pipe, FIFO (named pipe), and Shared Memory.

   The first process generates data and sends it to the second
   process using an unnamed pipe. The second process reads the
   data from the pipe, performs processing, and forwards the
   modified data to the third process using a FIFO. The third
   process then reads the data from the FIFO, performs further
   processing, and writes the final result into shared memory.

   The first process finally reads the processed data from
   shared memory and displays the output on standard output.

   The program demonstrates:
   1. Communication between related processes using pipe()
   2. Communication between processes using FIFO (mkfifo)
   3. Communication using shared memory (shmget, shmat)
   4. Data flow between multiple processes using different IPC
   5. Basic synchronization and process coordination
   6. Usage of system calls like fork(), read(), write(), exec()

   Problem Statement :
   Write a C program to establish communication between three
   processes using pipe, FIFO and shared memory. The first
   process should send data through a pipe to the second
   process. The second process should process and forward the
   data through FIFO to the third process. The third process
   should write the final processed data into shared memory,
   which is then read and displayed by the first process.
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Function to convert string to uppercase
void upperCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
}

int main()
{
    int pipe_fd[2];

    // Create a pipe
    pipe(pipe_fd);

    // Create child process
    int pid = fork();

    if (pid > 0)
    {
        // Parent process

        char str[50];

        // Take input string from user
        printf("Enter the string: ");
        scanf("%s", str);

        // Close read end of pipe
        close(pipe_fd[0]);

        // Write string to pipe
        write(pipe_fd[1], str, strlen(str) + 1);

        // Close write end of pipe
        close(pipe_fd[1]);

        // Wait for child process to finish
        wait(NULL);

        char buff[50];

        // Open FIFO to read final output
        int fd = open("/tmp/my_own_fifo", O_RDONLY);

        // Read data from FIFO
        int n = read(fd, buff, sizeof(buff) - 1);
        buff[n] = '\0';

        // Print final output
        printf("Final Output: %s\n", buff);

        // Close FIFO
        close(fd);
    }
    else if (pid == 0)
    {
        // Child process

        char str1[50];

        // Close write end of pipe
        close(pipe_fd[1]);

        // Read string from pipe
        read(pipe_fd[0], str1, sizeof(str1));

        // Close read end of pipe
        close(pipe_fd[0]);

        // Convert string to uppercase
        upperCase(str1);

        // Create shared memory
        int shmid = shmget('Z', 100, IPC_CREAT | 0644);

        // Attach shared memory
        char *shm = shmat(shmid, NULL, 0);

        // Write uppercase string to shared memory
        strcpy(shm, str1);

        // Detach shared memory
        shmdt(shm);
    }

    return 0;
}
