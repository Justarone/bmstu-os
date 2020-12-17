#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <sys/stat.h>
#include <sys/sem.h>

#define PERMS S_IRWXU | S_IRWXG | S_IRWXO
#define KEY IPC_PRIVATE
#define ITERATIONS_AMOUNT 50
#define WRITERS_AMOUNT 5
#define READERS_AMOUNT 6

#define SEMS_AMOUNT 4
#define ACT_RDRS 0
#define ACT_WRTR 1
#define READ_QUEUE 2
#define WRITE_QUEUE 3

#define FREE 1

#define SHMGET_FAILED 1
#define SHMAT_FAILED 2
#define FORK_FAILED 3
#define READER_LOCK_FAILED 4
#define READER_RELEASE_FAILED 5
#define WRITER_LOCK_FAILED 6
#define WRITER_RELEASE_FAILED 7
#define WRITE_FAILED 8
#define READ_FAILED 8
#define WAIT_FAILED 10
#define SHUTDOWN_FAILED 11
#define SEMGET_FAILED 12

#define TRUE 1
#define FALSE 0

// Time [TIME_FROM, TIME_FROM + TIME_RANGE)
#define TIME_FROM 1
#define TIME_RANGE 5

#endif // __CONSTANTS__
