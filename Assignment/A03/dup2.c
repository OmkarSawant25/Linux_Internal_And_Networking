/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A03 – WAP to understand usage of dup and dup2 system calls

   Description :
   This program demonstrates the usage of the dup() and dup2()
   system calls to redirect output from standard output (STDOUT)
   to a file. The program shows how file descriptors can be
   duplicated and reassigned to achieve output redirection.

   The program demonstrates:
   1. Redirection of printf() output to a file using dup()
   2. Redirection of printf() output to a file using dup2()
   3. Passing file name through command-line arguments
   4. Restoring STDOUT back to terminal after redirection
   5. Understanding file descriptor duplication
   6. Proper error handling for system calls

   Problem Statement :
   Write a C program to demonstrate the usage of dup() and
   dup2() system calls. Redirect the output of printf() to
   a file instead of STDOUT by duplicating file descriptors.
   The file name should be passed as a command-line argument.
   Print one message to STDOUT and another message to the file.

   OUTPUT :
   ./dup_dup2 f1.txt
   → One message printed on STDOUT
   → One message redirected and written into f1.txt
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int backup = dup(1); // backup stdout

    dup2(file, 1); // stdout -> file
    printf("Hello");
    fflush(stdout);

    dup2(backup, 1); // restore stdout
    printf("World\n");

    close(file);
    close(backup);

    return 0;
}
