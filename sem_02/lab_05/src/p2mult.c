#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
/*#include <stdio.h>*/

#define FILENAME "alphabet.txt"

void *run(void *arg) {
    /*printf("\n=== Separate thread start ===\n");*/
    int fd = *(int*)arg;
    int flag = 1;
    char c;
    while (flag == 1) {
        flag = read(fd, &c, 1);
        if (flag == 1)
            write(1, &c, 1);
    }
    /*printf("\n=== Separate thread end ===\n");*/
    return NULL;
}

int main() {
    /*setbuf(stdout, NULL);*/
    int fd1 = open(FILENAME, O_RDONLY);
    int fd2 = open(FILENAME, O_RDONLY);

    pthread_t td;
    pthread_create(&td, NULL, run, &fd2);
    /*printf("\n=== Main thread start ===\n");*/

    int flag = 1;
    char c;
    while (flag == 1) {
        flag = read(fd1, &c, 1);
        if (flag == 1)
            write(1, &c, 1);
    }
    /*printf("\n=== Main thread end ===\n");*/
    pthread_join(td, NULL);
    return 0;
}
