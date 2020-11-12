// wait
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define N 1
#define SECONDS 3

int main()
{
    printf("PARENT: PID=%d, GRP=%d\n", getpid(), getpgrp());
    int childpid;
    for (size_t i = 0; i < N; i++) {
        if ((childpid = fork()) == -1) {
            perror("Can’t fork.\n");
            return 1;
        } else if (childpid == 0) {
            printf("CHILD : PID=%d, GRP=%d, PPID=%d\n", getpid(), getppid(), getpgrp());
            sleep(SECONDS);
            return 0;
        }
    }

    for (size_t i = 0; i < N; i++) {
        int status;
        pid_t child_pid;
        child_pid = wait(&status);
        printf("Child has finished: PID = %d, status = %d\n", child_pid, status);
        int stat_val;
        if (WIFEXITED(stat_val))
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        else
            printf("Child terminated abnormally\n");
    }

    printf("PARENT IS GOING TO DIE!\n");
    return 0;
}
