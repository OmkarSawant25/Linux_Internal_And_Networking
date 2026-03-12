/*
   Name        : Omkar Ashok Sawant
   Program     : A15 – WAP to implement alarm with snooze for given time and date using SIGALRM

   Description :
   This program demonstrates implementation of an alarm system
   using signal handling and time-related system calls. The user
   provides the alarm time and optional date through command-line
   arguments. The program validates the entered time and sets an
   alarm using the alarm() system call and SIGALRM signal.

   When the alarm expires, a signal handler is executed which
   triggers an alarm message along with the scheduled date and
   time. The user is then prompted to either stop the alarm or
   snooze it for a specified duration. If snooze is selected,
   the alarm is reset for the entered snooze time (default
   1 minute if no value is provided).

   The program demonstrates:
   1. Handling SIGALRM signal using sigaction()
   2. Usage of alarm() and time-related functions
   3. Validation of user-provided time and date
   4. Implementing snooze functionality using signal reset
   5. Basic user interaction after signal handling
   6. Understanding asynchronous signal behaviour

   Problem Statement :
   Write a C program to implement an alarm system using SIGALRM.
   The program should accept time and optional date as command-line
   arguments, validate the inputs, and set an alarm. When the alarm
   triggers, display an alarm message and prompt the user to either
   stop or snooze the alarm. If snooze is selected, reset the alarm
   for the specified snooze duration.
*/

#include <stdio.h>
#define _XOPEN_SOURCE
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void print_alarm();

// signal handler for SIGALRM
void signal_handler(int signum)
{
    if (signum == SIGALRM)
    {
        print_alarm(); // call alarm display function
    }
}

// function executed when alarm expires
void print_alarm()
{
    int choice;

    printf("\nWake-up... Alarm!!\n");
    printf("Enter your choice:\n1. Snooze\n2. Exit\n");
    scanf("%d", &choice);

    if (choice == 1)
    {
        int min;
        printf("Enter snooze time (minutes): ");
        scanf("%d", &min);

        alarm(min * 60); // reset alarm for snooze time
    }
    else
    {
        exit(0); // terminate program
    }
}

int main(int argc, char *argv[])
{
    // check argument count
    if (argc < 2 || argc > 3)
    {
        printf("Error: No arguments passed\nUsage: ./alarm  [dd/mm/yy]\n");
        return -1;
    }

    signal(SIGALRM, signal_handler); // register signal handler

    // get current time
    time_t curr_time = time(NULL);
    struct tm *ptr = localtime(&curr_time);
    struct tm var = {0};

    printf("Current Time => %02d:%02d:%02d\n", ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

    // parse user time and optional date
    if (argc == 3)
    {
        strptime(argv[1], "%T", &var);
        strptime(argv[2], "%d/%m/%Y", &var);
    }
    else
    {
        strptime(argv[1], "%T", &var);

        // if date not given -> use today's date
        ptr = localtime(&curr_time);
        var.tm_mday = ptr->tm_mday;
        var.tm_mon = ptr->tm_mon;
        var.tm_year = ptr->tm_year;
    }

    // convert to seconds format
    time_t alarm_time = mktime(&var);

    // calculate difference
    double diff_time = alarm_time - curr_time;

    if (diff_time <= 0)
    {
        printf("Error: Invalid time\n");
    }
    else
    {
        alarm(diff_time); // set alarm
        while (1)
            pause(); // wait for signal
    }
}
