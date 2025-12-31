#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// int main()
// {
//     int pid, pid2, pid3, status;
//     pid = fork();

//     if (pid > 0)
//     {
//         printf("A child created with pid %d\n", pid);
//         pid2 = fork();
//         if (pid2 > 0)
//         {
//             printf("A child created with pid %d\n", pid2);
//             pid3 = fork();
//             if (pid3 > 0)
//             {
//                 printf("A child created with pid %d\n", pid3);
//                 while (waitpid(pid, &status, WNOHANG) == 0);
//                 if (WIFEXITED(status))
//                 {
//                     printf("Child %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
//                 }
//                 // else
//                 // {
//                 //     printf("Child %d terminated abnormally\n", pid);
//                 // }
//             }
//             else if (pid3 == 0)
//             {
//                 sleep(3);
//             }
//             else
//             {
//                 perror("fork");
//             }

//             while (waitpid(pid2, &status, WNOHANG) == 0)
//             // {
//                 // sleep(1);
//                 // printf("Parent is running\n");
//             // }
//             if (WIFEXITED(status))
//             {
//                 printf("Child %d terminated normally with exit status %d parent terminating\n", pid2, WEXITSTATUS(status));
//             }
//             // else
//             // {
//             //     printf("Child %d terminated abnormally\n", pid2);
//             // }
//         }
//         else if (pid2 == 0)
//         {
//             sleep(5);
//         }
//         else
//         {
//             perror("fork");
//         }
//         while (waitpid(pid3, &status, WNOHANG) == 0)
//         {
//             // sleep(1);
//             // printf("Parent is running\n");
//         }
//         if (WIFEXITED(status))
//         {
//             printf("Child %d terminated normally with exit status %d parent terminating\n", pid3, WEXITSTATUS(status));
//         }
//         // else
//         // {
//         //     printf("Child %d terminated abnormally\n", pid3);
//         // }
//     }
//     else if (pid == 0)
//     {
//         sleep(5);
//     }
//     else
//     {
//         perror("fork");
//     }
//     return 0;
// }

int main()
{
    int pid1, pid2, pid3, status;
    pid1 = fork();

    if (pid1 > 0)
    {
        printf("A child created with pid %d\n", pid1);
        pid2 = fork();
        {
            if (pid2 > 0)
            {
                printf("A child created with pid %d\n", pid2);
            }
        }
    }
    return 0;
}