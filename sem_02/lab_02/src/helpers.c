#include "helpers.h"

int myfunc(const char* filename, int offset) {
    printf(" ");
    for (size_t i = 0; i < offset; i++)
        printf("--->");
    printf(" %s\n", filename);
    return 0;
}

char* concat(char* path, char* new_dir) {
    int n = strlen(path);
    char* new_path = malloc(n + strlen(new_dir) + 2);
    if (new_path == NULL) {
        printf("Memory error!");
        exit(1);
    } else {
        strcpy(new_path, path);
        new_path[n++] = '/';
        strcpy(new_path + n, new_dir);
        return new_path;
    }
}
