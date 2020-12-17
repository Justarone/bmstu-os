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
#include "io_objects.h"

int main(void) {
    setbuf(stdout, NULL);
    int fd = shmget(KEY, sizeof(int), IPC_CREAT | PERMS);
    if (fd == -1) {
        perror("Failed to create shared memory!");
        return SHMGET_FAILED;
    }

    int* shared_counter;
    if ((shared_counter = (int*)shmat(fd, 0, 0)) == (void*)-1) {
        perror("Failed to shmat!");
        return SHMAT_FAILED;
    }

    int sid = semget(KEY, SEMS_AMOUNT, IPC_CREAT | PERMS);
    if (sid == -1) {
        perror("Can't create array of semaphores!");
        return SEMGET_FAILED;
    }

    semctl(sid, ACT_RDRS, SETVAL, 0);
    semctl(sid, ACT_WRTR, SETVAL, 0);
    semctl(sid, WRITE_QUEUE, SETVAL, 0);
    semctl(sid, READ_QUEUE, SETVAL, 0);

    int child_pid;
    for (size_t i = 0; i < READERS_AMOUNT; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for producer!");
            exit(FORK_FAILED);
            break;
        case 0:
            run_reader(shared_counter, sid, i);
            return 0;
        }
    }

    for (int i = 0; i < WRITERS_AMOUNT; i++) {
        switch ((child_pid = fork())) {
        case -1:
            perror("Failed to fork for consumer!");
            exit(FORK_FAILED);
            break;
        case 0:
            run_writer(shared_counter, sid, i);
            return 0;
        }
    }

    for (size_t i = 0; i < WRITERS_AMOUNT + READERS_AMOUNT; i++) {
        int status;
        if (wait(&status) == -1) {
            perror("Something wrong with children waiting!");
            exit(WAIT_FAILED);
        }
        if (!WIFEXITED(status))
            printf("One of children terminated abnormally\n");
    }
    
    if (shmdt((void*)shared_counter) == -1 
            || shmctl(fd, IPC_RMID, NULL) == -1 
            || semctl(sid, IPC_RMID, 0) == -1) {
        perror("Something went wrong during shutdown!");
        return SHUTDOWN_FAILED;
    }

    return 0;
}
