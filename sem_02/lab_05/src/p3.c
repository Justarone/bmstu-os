#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

#define FILENAME "out.txt"

void print_stat(char *filename) {
    struct stat buff;
    int rc = stat(filename, &buff);
    if (rc == -1) {
        printf("Can't get stat\n");
        return;
    }
    printf("size: %lu, inode: %lu\n", buff.st_size, buff.st_ino);
}

int main() {
    FILE *fd1 = fopen(FILENAME, "w");
    print_stat(FILENAME);
    FILE *fd2 = fopen(FILENAME, "w");
    print_stat(FILENAME);
    /*fseek(fd2, 1, SEEK_SET);*/
    for (char c = 'a'; c <= 'z'; c++) {
        if (c % 2) {
            fprintf(fd1, "%c", c);
        } else {
            fprintf(fd2, "%c", c);
        }
    }
    /*printf("from 'a': %d, from 'b': %d\n", fileno(fd1), fileno(fd2));*/
    fclose(fd1);
    print_stat(FILENAME);
    fclose(fd2);
    print_stat(FILENAME);
    return 0;
}
