#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

#define BUF_SIZE 20
#define FILENAME "alphabet.txt"

void *run(void *arg) {
    /*printf("\n=== Separate thread start ===\n");*/
    FILE *fs = arg;
    int flag = 1;
    char c;

    while (flag == 1) {
        flag = fscanf(fs, "%c", &c);
        if (flag == 1)
            fprintf(stdout, "t2: %c\n", c);
    }
    /*printf("\n=== Separate thread end ===\n");*/
    return NULL;
}

int main() {
    /*setbuf(stdout, NULL);*/
    int fd = open(FILENAME, O_RDONLY);
    pthread_t td;

    FILE* fs1 = fdopen(fd, "r");
    char buff1[BUF_SIZE];
    setvbuf(fs1, buff1, _IOFBF, BUF_SIZE);

    FILE* fs2 = fdopen(fd, "r");
    char buff2[BUF_SIZE];
    setvbuf(fs2, buff2, _IOFBF, BUF_SIZE);

    pthread_create(&td, NULL, run, fs2);

    /*printf("\n=== Main thread start ===\n");*/

    int flag = 1;
    char c;
    while (flag == 1) {
        flag = fscanf(fs1, "%c", &c);
        if (flag == 1)
            fprintf(stdout, "t1: %c\n", c);
    }

    /*printf("\n=== Main thread end ===\n");*/

    pthread_join(td, NULL);
    return 0;
}
