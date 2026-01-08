/*
   Name        : Omkar Ashok Sawant
   Date        : 02/01/2026
   Program     : A02 – Implement a wc (word count) command with -l, -w, -c options

   Description :
   This program implements a simplified version of the Linux
   `wc` (word count) command using basic file system calls.
   It counts the number of lines, words, and characters
   (bytes) from files passed through command-line arguments.
   If no file is provided, the program reads input from
   standard input until EOF (Ctrl + D).

   The program demonstrates:
   1. Handling command-line arguments and options using getopt()
   2. Reading data using file descriptors (open, read, close)
   3. Counting lines, words, and bytes from files or stdin
   4. Processing multiple files and displaying individual
      counts along with total counts
   5. Selective output using -l, -w, and -c options
   6. Proper error handling for system calls

   Problem Statement :
   Write a C program to count the number of lines, words, and
   characters (bytes) in one or more files using system calls.
   Implement a word_count() function that reads from a file
   descriptor and updates line, word, and byte counts via
   pass-by-reference. The main function should open files,
   call the word_count() function, and print results.
   If no file is provided, read from standard input until EOF.
   Support -l, -w, and -c options individually or in
   combination.

   OUTPUT :
   ./word_count
   → Reads from stdin till EOF and prints line, word, and byte count

   ./word_count file.txt
   → Prints line, word, and byte count of file.txt

   ./word_count f1.txt f2.txt
   → Prints counts for each file and total count

   ./word_count file.txt -l -w
   → Prints only line and word count
*/

#include <stdio.h>
#include <unistd.h> // getopt
#include <fcntl.h>
#include <stdbool.h>

extern int optind;
void my_wc(int fd, int *line_count, int *word_count, int *char_count)
{
    char ch, prev = ' ';
    while (read(fd, &ch, 1) != 0)
    {
        (*char_count)++;
        if (ch == '\n')
            (*line_count)++;
        if ((ch == ' ' || ch == '\n' || ch == '\t') && (prev != ' ' && prev != '\n' && prev != '\t'))
            (*word_count)++;

        prev = ch;
    }
    if (prev != ' ' && prev != '\n' && prev != '\t')
        (*word_count)++;
}

int main(int argc, char *argv[])
{
    int ret;
    bool line_flag = 0;
    bool char_flag = 0;
    bool word_flag = 0;

    int line_count = 0;
    int word_count = 0;
    int char_count = 0;
    int file_count = 0;

    int line_total = 0;
    int word_total = 0;
    int char_total = 0;

    while ((ret = getopt(argc, argv, "lwc")) != -1)
    {
        if (ret == 'l')
            line_flag = 1;
        else if (ret == 'w')
            word_flag = 1;
        else if (ret == 'c')
            char_flag = 1;
    }
    // printf("Pos is %d\n", optind);

    for (int i = optind; argv[i] != NULL; i++)
    {
        line_count = 0;
        word_count = 0;
        char_count = 0;

        int fd = open(argv[i], O_RDONLY);
        my_wc(fd, &line_count, &word_count, &char_count);

        line_total += line_count;
        word_total += word_count;
        char_total += char_count;
        file_count++;

        if (line_flag == 0 && word_flag == 0 && char_flag == 0)
        {
            printf("%5d%5d%5d%12s\n", line_count, word_count, char_count, argv[i]);
            continue;
        }
        if (line_flag)
        {
            printf("%5d", line_count);
        }
        if (word_flag)
        {
            printf("%5d", word_count);
        }
        if (char_flag)
        {
            printf("%5d", char_count);
        }
        printf("%12s\n", argv[i]);
    }
    if (file_count > 1)
    {
        printf("%5d%5d%5d%12s\n", line_total, word_total, char_total, "total");
    }

    if (argv[optind] == NULL)
    {
        my_wc(0, &line_count, &word_count, &char_count);
        printf("%5d%5d%5d\n", line_count, word_count, char_count);
    }

    return 0;
}
