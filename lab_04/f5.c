// signal
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define BUFFER_LEN 100
#define SECONDS 3
#define N 2
#define SEND_MODE 1
#define QUITE_MODE 0

const char* SECRET_MSGS[2] = { "secret message 1", "secret message 2" };
int MODES[N] = { QUITE_MODE };

void err_sys(const char* x)
{
    perror(x);
    exit(1);
}

void child_callback0(int sig_number) {
    MODES[0] = SEND_MODE;
}

void child_callback1(int sig_number) {
    MODES[1] = SEND_MODE;
}

typedef void (*cb_ptr)(int);
cb_ptr child_callbacks[2] = { child_callback0, child_callback1 };

int main()
{
    int fd[2];
    int pid;
    pipe(fd);

    for (size_t i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            err_sys("Error fork()");
        } else if (pid == 0) {
            close(fd[0]);
            signal(SIGINT, child_callbacks[i]);
            sleep(SECONDS);
            if (MODES[i]) {
                size_t written = write(fd[1], SECRET_MSGS[i], strlen(SECRET_MSGS[i]));
                printf("Sent message to parent! (written %zu bytes)\n", written);
            }
            return 0;
        }
    }

    signal(SIGINT, SIG_IGN);

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

    close(fd[1]);
    char buffer[BUFFER_LEN] = { 0 };
    size_t buf_len = read(fd[0], buffer, BUFFER_LEN);
    printf("Received message (size: %zu): %s!\n", buf_len, buffer);

    return 0;
}
