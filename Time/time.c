// man 3 time → library call
// time_t stores seconds since 1 Jan 1970
// Both methods are equivalent
// %ld is used because time_t is usually long

#include <stdio.h>
#include <time.h>

int main()
{
    time_t usr_time;

    /* Method 1 */
    time(&usr_time);

    /* Method 2 */
    usr_time = time(NULL);

    printf("The EPOCH time is => %ld\n", usr_time);

    return 0;
}
