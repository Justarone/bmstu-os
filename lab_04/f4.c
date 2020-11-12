#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_LEN 100
const char* SECRET_MSG = "secret message";

void err_sys(const char* x)
{
    perror(x);
    exit(1);
}

int main()
{
    int fd[2];
    int pid;
    pipe(fd);

    if ((pid = fork()) < 0) {
        err_sys("Error fork()");
    } else if (pid == 0) {
        close(fd[0]);
        size_t written = write(fd[1], SECRET_MSG, strlen(SECRET_MSG));
        printf("Sent message to parent! (written %zu bytes)\n", written);
        exit(0);
    } else {
        char buffer[BUFFER_LEN] = { 0 };
        close(fd[1]);
        size_t buf_len = read(fd[0], buffer, BUFFER_LEN);
        printf("Received message (size: %zu): %s!\n", buf_len, buffer);
    }

    int status;
    pid_t child_pid;
    child_pid = wait(&status);
    printf("Child has finished: PID = %d, status = %d\n", child_pid, status);
    int stat_val;
    if (WIFEXITED(stat_val))
        printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
    else
        printf("Child terminated abnormally\n");

    return 0;
}
