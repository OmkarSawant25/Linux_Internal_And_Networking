// man 2 gettimeofday → system call
// tv_sec → seconds since epoch
// tv_usec → microseconds part
// Gives higher precision than time()

#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval usr_time;

    gettimeofday(&usr_time, NULL);

    printf("The EPOCH time in seconds      : %ld\n", usr_time.tv_sec);
    printf("The EPOCH time in microseconds : %ld\n", usr_time.tv_usec);

    return 0;
}
