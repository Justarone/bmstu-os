#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include "constants.h"
#include "buffer.h"
#include "runners.h"

int main(void) {
    setbuf(stdout, NULL);
    int fd = shmget(KEY, sizeof(CBuffer), IPC_CREAT | PERMS);
    if (fd == -1) {
        perror("Failed to create shared memory!");
        return SHMGET_FAILED;
    }

    CBuffer *buffer;
    if ((buffer = (CBuffer*)shmat(fd, 0, 0)) == (void*)-1) {
        perror("Failed to shmat!");
        return SHMAT_FAILED;
    }

    if (init_cbuffer(buffer) == -1) {
        perror("Failed to init buffer!");
        return SHMAT_FAILED;
    }

    int sid = semget(KEY, SEMS_AMOUNT, IPC_CREAT | PERMS);
    if (sid == -1) {
        perror("Can't create array of semaphores!");
        return SEMGET_FAILED;
    }
    semctl(sid, BIN_SEM, SETVAL, FREE);
    semctl(sid, BUF_EMPTY, SETVAL, N);
    semctl(sid, BUF_FULL, SETVAL, 0);

    int child_pid;
    for (size_t i = 0; i < PRODUCERS_AMOUNT; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for producer!");
            exit(FORK_FAILED);
            break;
        case 0:
            run_producer(buffer, sid, i);
            return 0;
        }
    }

    for (int i = 0; i < CONSUMERS_AMOUNT; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for consumer!");
            exit(FORK_FAILED);
            break;
        case 0:
            run_consumer(buffer, sid, i);
            return 0;
        }
    }

    for (size_t i = 0; i < CONSUMERS_AMOUNT + PRODUCERS_AMOUNT; i++) {
        int status;
        if (wait(&status) == -1) {
            perror("Something wrong with children waiting!");
            exit(WAIT_FAILED);
        }
        if (!WIFEXITED(status))
            printf("One of children terminated abnormally\n");
    }
    
    if (shmdt((void*)buffer) == -1 || shmctl(fd, IPC_RMID, NULL) == -1 || semctl(sid, IPC_RMID, 0) == -1) {
        perror("Something went wrong during shutdown!");
        return SHUTDOWN_FAILED;
    }

    return 0;
}
