#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define READERS_AMOUNT 5
#define WRITERS_AMOUNT 3
#define WRITE_ITERATIONS 8
#define READ_ITERATIONS 7
#define WRITE_TIMEOUT 300
#define READ_TIMEOUT 300
#define DIFF 4000

#define CREATE_MUTEX_FAILED 1
#define CREATE_EVENT_FAILED 2
#define CREATE_THREAD_FAILED 3

HANDLE mutex;
HANDLE can_read;
HANDLE can_write;
LONG waiting_writers = 0;
LONG waiting_readers = 0;
LONG active_readers = 0;
bool active_writer = false;

int value = 0;

void start_read(void) {
    InterlockedIncrement(&waiting_readers);
    // WaitForSingleObject(object, how_long_wait)
    if (active_writer || (WaitForSingleObject(can_write, 0) == WAIT_OBJECT_0 && waiting_writers)) 
        WaitForSingleObject(can_read, INFINITE);
    // fake mutex
    WaitForSingleObject(mutex, INFINITE);
    InterlockedDecrement(&waiting_readers);
    InterlockedIncrement(&active_readers);
    SetEvent(can_read);
    ReleaseMutex(mutex);
}

void stop_read(void) {
    InterlockedDecrement(&active_readers);
    if (active_readers == 0) {
        ResetEvent(can_read);
        SetEvent(can_write);
    }
}

DWORD WINAPI run_reader(CONST LPVOID lpParams) {
    int index = (int)lpParams;
    int sleep_time;
    srand(time(NULL) + index);
    for (size_t i = 0; i < READ_ITERATIONS; i++) {
        sleep_time = READ_TIMEOUT + rand() % DIFF;
        Sleep(sleep_time);
        start_read();
        printf("  Reader #%ld read:  %5ld (slept %4d ms)\n", index, value, sleep_time);
        stop_read();
    }
    return 0;
}

void start_write(void) {
    InterlockedIncrement(&waiting_writers);
    if (active_writer || active_readers > 0) 
        WaitForSingleObject(can_write, INFINITE);
    InterlockedDecrement(&waiting_writers);
    active_writer = true;
}

void stop_write(void) {
    active_writer = false;
    if (waiting_readers) 
        SetEvent(can_read);
    else 
        SetEvent(can_write);
}

DWORD WINAPI run_writer(CONST LPVOID lpParams) {
    int index = (int)lpParams;
    int sleep_time;
    srand(time(NULL) + index + READERS_AMOUNT);
    for (int i = 0; i < WRITE_ITERATIONS; ++i) {
        sleep_time = WRITE_TIMEOUT + rand() % DIFF;
        Sleep(sleep_time);
        start_write();
        ++value;
        printf("  Writer #%ld write: %5ld (slept %4d ms)\n", index, value, sleep_time);
        stop_write();
    }
    return 0;
}

int main(void) {
    setbuf(stdout, NULL);
    HANDLE readers_threads[READERS_AMOUNT];
    HANDLE writers_threads[WRITERS_AMOUNT];
    // CreateMutex(attr, lock_now?, name) (attr have to be NULL (docs.windows))
    if ((mutex = CreateMutex(NULL, FALSE, NULL)) == NULL) {
        perror("Failed call of CreateMutex");
        return CREATE_MUTEX_FAILED;
    }
    // CreateEvent(attr, manually?, init_state, name)
    if ((can_read = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL 
            || (can_write = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL) {
        perror("Failed call of CreateEvent");
        return CREATE_EVENT_FAILED;
    }
    // CreateThread(attr, stack_size, begin_func, func_param, flags, pointer_where_to_return_id)
    for (int i = 0; i < READERS_AMOUNT; ++i) 
        if ((readers_threads[i] = CreateThread(NULL, 0, run_reader, (LPVOID)i, 0, NULL)) == NULL) {
            perror("Failed call of CreateThread");
            return CREATE_THREAD_FAILED;
    }
    for (int i = 0; i < WRITERS_AMOUNT; i++) 
        if ((writers_threads[i] = CreateThread(NULL, 0, run_writer, (LPVOID)i, 0, NULL)) == NULL) {
            perror("Failed call of CreateThread");
            return CREATE_THREAD_FAILED;
        }

    // WaitForMultipleObjects(array_size, pointer_to_array, all?, how_long_wait)
    WaitForMultipleObjects(READERS_AMOUNT, readers_threads, TRUE, INFINITE);
    WaitForMultipleObjects(WRITERS_AMOUNT, writers_threads, TRUE, INFINITE);
    CloseHandle(mutex);
    CloseHandle(can_read);
    CloseHandle(can_write);

    return 0;
}
