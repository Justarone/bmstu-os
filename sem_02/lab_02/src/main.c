#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "walk_dir.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("You should choose one dir to create a dirtree\n");
        exit(1);
    }
    return walk_dir(argv[1]);
}
