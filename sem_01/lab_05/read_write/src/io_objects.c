#include "io_objects.h"

struct sembuf READER_QUEUE[] = { 
    { READ_QUEUE, 1, 0 }, // add member to read queue
    { ACT_WRTR, 0, 0 }, // No active writers
    { WRITE_QUEUE, 0, 0 }, // wait every member of write queue
};

struct sembuf READER_LOCK[] = { 
    { ACT_RDRS, 1, 0 }, // inc active readers
    { READ_QUEUE, -1, 0 }, // leave queue (-1 in readers queue)
};
struct sembuf READER_RELEASE[] = { 
    { ACT_RDRS, -1, 0 }, // -1 active reader
};

struct sembuf WRITER_QUEUE[] = { 
    { WRITE_QUEUE, 1, 0 }, // add member to write queue
    { ACT_RDRS, 0, 0 }, // No active readers
    { ACT_WRTR, 0, 0 }, // No active writers
};

struct sembuf WRITER_LOCK[] = {  
    { ACT_WRTR, 1, 0 }, // inc active writers
    { WRITE_QUEUE, -1, 0 }, // leave queue (-1 in writers queue)
};

struct sembuf WRITER_RELEASE[] = { 
    { ACT_WRTR, -1, 0 }, // -1 active writer
}; 

int start_read(int sid) {
    return semop(sid, READER_QUEUE, 3) != -1 && semop(sid, READER_LOCK, 2) != -1;
}

int stop_read(int sid) {
    return semop(sid, READER_RELEASE, 1) != -1;
}

int run_reader(int* const shared_counter, const int sid, const int reader_id)
{
    if (!shared_counter)
        return -1;

    srand(time(NULL) + reader_id);

    int sleep_time;
    for (int i = 0; i < ITERATIONS_AMOUNT; i++) {
        sleep_time = rand() % TIME_RANGE + TIME_FROM;
        sleep(sleep_time);

        if (!start_read(sid)) {
            perror("Something went wrong with start_read!");
            exit(READER_LOCK_FAILED);
        }
        // critical section
        int val = *shared_counter;
        printf(" \e[1;31mReader #%d read:  %5d\e[0m (slept %ds)\n", reader_id, val, sleep_time);
        // critical section ends
        if (!stop_read(sid)) {
            perror("Something went wrong with stop_read!");
            exit(READER_RELEASE_FAILED);
        }
    }
    return 0;
}

int start_write(int sid) {
    return semop(sid, WRITER_QUEUE, 3) != -1 && semop(sid, WRITER_LOCK, 2) != -1;
}

int stop_write(int sid) {
    return semop(sid, WRITER_RELEASE, 1) != -1;
}

int run_writer(int* const shared_counter, const int sid, const int writer_id)
{
    if (!shared_counter)
        return -1;

    srand(time(NULL) + writer_id + READERS_AMOUNT);

    int sleep_time;
    for (int i = 0; i < ITERATIONS_AMOUNT; i++) {
        sleep_time = rand() % TIME_RANGE + TIME_FROM;
        sleep(sleep_time);

        if (!start_write(sid)) {
            perror("Something went wrong with start_write!");
            exit(WRITER_LOCK_FAILED);
        }
        // critical section
        int val = ++(*shared_counter);
        printf(" \e[1;32mWriter #%d write: %5d\e[0m (slept %ds)\n", writer_id, val, sleep_time);
        // critical section ends
        if (!stop_write(sid)) {
            perror("Something went wrong with stop_write!");
            exit(WRITER_RELEASE_FAILED);
        }
    }
    return 0;
}
