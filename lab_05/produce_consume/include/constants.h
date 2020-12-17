#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <sys/stat.h>
#include <sys/sem.h>

#define PERMS S_IRWXU | S_IRWXG | S_IRWXO
#define KEY IPC_PRIVATE
#define N 64
#define ITERATIONS_AMOUNT 5
#define CONSUMERS_AMOUNT 3
#define PRODUCERS_AMOUNT 3

#define SEMS_AMOUNT 3
#define BIN_SEM 0
#define BUF_FULL 1
#define BUF_EMPTY 2

#define FREE 1

#define SHMGET_FAILED 1
#define SHMAT_FAILED 2
#define FORK_FAILED 3
#define PRODUCE_LOCK_FAILED 4
#define PRODUCE_RELEASE_FAILED 5
#define CONSUME_LOCK_FAILED 6
#define CONSUME_RELEASE_FAILED 7
#define BUFFER_WRITE_FAILED 8
#define BUFFER_READ_FAILED 9
#define WAIT_FAILED 10
#define SHUTDOWN_FAILED 11
#define SEMGET_FAILED 12

#define TRUE 1
#define FALSE 0

// Time [TIME_FROM, TIME_FROM + TIME_RANGE)
#define TIME_FROM 1
#define TIME_RANGE 5

#endif // __CONSTANTS__
