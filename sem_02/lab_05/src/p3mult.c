#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define FILENAME "out.txt"

void *run(void *arg) {
    /*printf("\n=== Separate thread start ===\n");*/
    FILE *fd2 = arg;
    for (char c = 'b'; c <= 'z'; c += 2) {
        fprintf(fd2, "%c", c);
    }
    fclose(fd2);
    /*printf("\n=== Separate thread end ===\n");*/
    return NULL;
}

int main() {
    FILE *fd1 = fopen(FILENAME, "w");
    FILE *fd2 = fopen(FILENAME, "w");

    pthread_t td;
    pthread_create(&td, NULL, run, fd2);

    /*printf("\n=== Main thread start ===\n");*/
    for (char c = 'a'; c <= 'z'; c += 2) {
        fprintf(fd1, "%c", c);
    }
    /*sleep(1);*/
    /*printf("\n=== Main thread end ===\n");*/

    fclose(fd1);
    pthread_join(td, NULL);
    return 0;
}
