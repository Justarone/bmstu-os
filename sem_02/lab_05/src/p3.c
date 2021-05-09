#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define FILENAME "out.txt"

int main() {
    FILE *fd1 = fopen(FILENAME, "w");
    FILE *fd2 = fopen(FILENAME, "w");
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
    fclose(fd2);
    return 0;
}
