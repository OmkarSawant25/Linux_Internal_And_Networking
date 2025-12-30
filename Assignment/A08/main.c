#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid[3];
    int status;

    for (int i = 0; i < 3; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {
            printf("Child %d with pid %d created\n", i + 1, getpid());
            sleep(2);
            return 0;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        waitpid(pid[i], &status, 0);

        if (WIFEXITED(status))
        {
            printf("Child %d is terminated with code %d\n", pid[i], WEXITSTATUS(status));
        }
    }

    return 0;
}

// int main()
// {
//     int pid, status, status2, status3;
//     pid = fork();

//     if (pid > 0)
//     {
//         printf("A child created with pid %d\n", pid);
//         int pid2 = fork();
//         if (pid2 > 0)
//         {
//             printf("A child created with pid %d\n", pid2);
//             int pid3 = fork();
//             if (pid3 > 0)
//             {
//                 printf("A child created with pid %d\n", pid3);
//                 while (waitpid(pid3, &status3, WNOHANG) == 0)
//                 {
//                     // sleep(1);
//                     // printf("Parent is running\n");
//                 }
//                 if (WIFEXITED(status3))
//                 {
//                     printf("Child %d terminated normally with exit status %d parent terminating\n", pid3, WEXITSTATUS(status3));
//                 }
//                 else
//                 {
//                     printf("Child %d terminated abnormally\n", pid3);
//                 }
//             }
//             else if(pid3 == 0)
//             {
//                 sleep(5);
//             }
//             else
//             {
//                 perror("fork");
//             }

//             while (waitpid(pid2, &status2, WNOHANG) == 0)
//             {
//                 // sleep(1);
//                 // printf("Parent is running\n");
//             }
//             if (WIFEXITED(status2))
//             {
//                 printf("Child %d terminated normally with exit status %d parent terminating\n", pid2, WEXITSTATUS(status2));
//             }
//             else
//             {
//                 printf("Child %d terminated abnormally\n", pid2);
//             }
//         }
//         else if (pid2 == 0)
//         {
//             sleep(5);
//         }
//         else
//         {
//             perror("fork");
//         }
//         while (waitpid(pid, &status, WNOHANG) == 0)
//         {
//             // sleep(1);
//             // printf("Parent is running\n");
//         }
//         if (WIFEXITED(status))
//         {
//             printf("Child %d terminated normally with exit status %d parent terminating\n", pid, WEXITSTATUS(status));
//         }
//         else
//         {
//             printf("Child %d terminated abnormally\n", pid);
//         }
//     }
//     else if (pid == 0)
//     {
//         sleep(5);
//     }
//     else
//     {
//         perror("fork");
//     }
// }