/*
Name        : Omkar Ashok Sawant
Date        : 17/12/2025
Description : A01 - Implement a cp(copy) command with –p option
*/

#include <stdio.h>    // Standard I/O
#include <unistd.h>   // read(), write(), close()
#include <fcntl.h>    // open() flags
#include <errno.h>    // errno values
#include <sys/stat.h> // stat, fstat, fchmod
#include <string.h>   // strcmp()

// Copy data from source file to destination file
void my_cp(int src_fd, int dest_fd)
{
    char ch;
    // Read and write one byte at a time
    while (read(src_fd, &ch, 1) > 0)
        write(dest_fd, &ch, 1);
}

// Copy file permissions from source to destination
void cp_permission(int src_fd, int dest_fd)
{
    struct stat var;
    // Get source file permissions
    fstat(src_fd, &var);
    // Apply same permissions to destination file
    fchmod(dest_fd, var.st_mode);
}

int main(int argc, char *argv[])
{
    // Allow only 3 or 4 arguments
    if (argc == 3 || argc == 4)
    {
        int src_fd;  // Source file descriptor
        int dest_fd; // Destination file descriptor

        // Open source file
        if (strcmp(argv[1], "-p") == 0)
            src_fd = open(argv[2], O_RDONLY);
        else
            src_fd = open(argv[1], O_RDONLY);

        // Check source file open error
        if (src_fd == -1)
        {
            perror("Open");
            return 1;
        }

        // Open destination file
        if (strcmp(argv[1], "-p") == 0)
            dest_fd = open(argv[3], O_CREAT | O_EXCL | O_WRONLY, 0644);
        else
            dest_fd = open(argv[2], O_CREAT | O_EXCL | O_WRONLY, 0644);

        // If destination file already exists
        if (dest_fd == -1)
        {
            if (errno == EEXIST)
            {
                char ch;
                // Ask user for overwrite permission
                fprintf(stdout, "File %s Already Exist\n", argv[2]);
                printf("Do you want to Overwrite this file (Yy/Nn)\n");
                scanf("%c", &ch);

                // If user agrees to overwrite
                if (ch == 'Y' || ch == 'y')
                {
                    if (strcmp(argv[1], "-p") == 0)
                    {
                        // Truncate destination file
                        dest_fd = open(argv[3], O_TRUNC | O_WRONLY);
                        my_cp(src_fd, dest_fd);         // Copy data
                        cp_permission(src_fd, dest_fd); // Copy permissions
                    }
                    else
                    {
                        // Truncate destination file
                        dest_fd = open(argv[2], O_TRUNC | O_WRONLY);
                        my_cp(src_fd, dest_fd); // Copy data
                    }
                }
                else
                {
                    // User denied overwrite
                    printf("Error: Failed to copy file\n");
                }
            }
        }
        else
        {
            if (strcmp(argv[1], "-p") == 0)
            {
                dest_fd = open(argv[3], O_WRONLY);
                my_cp(src_fd, dest_fd);         // Copy data
                cp_permission(src_fd, dest_fd); // Copy permissions
            }
            else
            {
                dest_fd = open(argv[2], O_WRONLY);
                my_cp(src_fd, dest_fd); // Copy data
            }
        }

        // Close file descriptors
        close(src_fd);
        close(dest_fd);
    }
    else
    {
        // Wrong usage
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    return 0;
}
