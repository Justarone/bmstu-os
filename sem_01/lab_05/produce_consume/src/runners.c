#include "runners.h"

struct sembuf PRODUCE_LOCK[2] = { { BUF_EMPTY, -1, 0 }, { BIN_SEM, -1, 0 } };
struct sembuf PRODUCE_RELEASE[2] = { { BUF_FULL, 1, 0 }, { BIN_SEM, 1, 0 } };

struct sembuf CONSUME_LOCK[2] = { { BUF_FULL, -1, 0 }, { BIN_SEM, -1, 0 } };
struct sembuf CONSUME_RELEASE[2] = { { BUF_EMPTY, 1, 0 }, { BIN_SEM, 1, 0 } };

int run_producer(CBuffer* const buffer, const int sid, const int prod_id) {
    if (!buffer)
        return -1;

    srand(time(NULL) + prod_id);

    int sleep_time;
    char ch;

    for (int i = 0; i < ITERATIONS_AMOUNT; i++) {
        sleep_time = rand() % PTIME_RANGE + PTIME_FROM;
        sleep(sleep_time);

        if (semop(sid, PRODUCE_LOCK, 2) == -1) {
            perror("Something went wrong with produce lock!");
            exit(PRODUCE_LOCK_FAILED);
        }
        // critical section
        ch = 'a' + (char)(buffer->wpos % 26);
        if (write_cbuffer(buffer, ch) == -1) {
            perror("Something went wrong with buffer writing!");
            return BUFFER_WRITE_FAILED;
        }
        printf(" \e[1;32mProducer #%d write: %c\e[0m (slept %ds)\n", prod_id, ch, sleep_time);
        // critical section ends
        if (semop(sid, PRODUCE_RELEASE, 2) == -1) {
            perror("Something went wrong with produce release!");
            exit(PRODUCE_RELEASE_FAILED);
        }

    }
    return 0;
}

int run_consumer(CBuffer* const buffer, const int sid, const int cons_id) {
    if (!buffer)
        return -1;

    srand(time(NULL) + cons_id + PRODUCERS_AMOUNT);

    int sleep_time;
    char ch;

    for (int i = 0; i < ITERATIONS_AMOUNT; i++) {
        sleep_time = rand() % CTIME_RANGE + CTIME_FROM;
        sleep(sleep_time);

        if (semop(sid, CONSUME_LOCK, 2) == -1) {
            perror("Something went wrong with consume lock!");
            exit(CONSUME_LOCK_FAILED);
        }
        // critical section
        if (read_cbuffer(buffer, &ch) == -1) {
            perror("Something went wrong with buffer reading!");
            return BUFFER_READ_FAILED;
        }
        printf(" \e[1;31mConsumer #%d read:  %c\e[0m (slept %ds)\n", cons_id, ch, sleep_time);
        // critical section ends
        if (semop(sid, CONSUME_RELEASE, 2) == -1) {
            perror("Something went wrong with write release!");
            exit(CONSUME_RELEASE_FAILED);
        }

    }
    return 0;
}


