#include <stdio.h>
#define _XOPEN_SOURCE
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void print_alarm();

void signal_handler(int signum)
{
    if (signum == SIGALRM)
    {
        print_alarm();
    }
}

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
        alarm(min * 60);
    }
    else
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        printf("Error: No arguments passed\nUsage: ./alarm  [dd/mm/yy]\n");
        return -1;
    }

    signal(SIGALRM, signal_handler);

    time_t curr_time = time(NULL);
    struct tm *ptr = localtime(&curr_time);
    struct tm var = {0};

    printf("Current Time => %02d:%02d:%02d\n", ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

    if (argc == 3)
    {
        strptime(argv[1], "%T", &var);
        strptime(argv[2], "%d/%m/%Y", &var);
    }
    else
    {
        strptime(argv[1], "%T", &var);
        ptr = localtime(&curr_time);
        var.tm_mday = ptr->tm_mday;
        var.tm_mon = ptr->tm_mon;
        var.tm_year = ptr->tm_year;
    }

    time_t alarm_time = mktime(&var);
    double diff_time = alarm_time - curr_time;

    if (diff_time <= 0)
    {
        printf("Error: Invalid time\n");
    }
    else
    {
        alarm(diff_time);
        while (1)
            pause();
    }
}