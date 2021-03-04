#include "walk_dir.h"

char* to_owned(char* pathname) {
    char *fullpath = malloc(PATH_MAX + 1);
    if (!fullpath) {
        printf("Memory error!");
        exit(1);
    }
    strcpy(fullpath, pathname);
    return fullpath;
}

int walk_dir(char* pathname) {
    struct dirent* dirp;
    DIR* dp;
    int nlevel = 0;
    stack_item_t *stack = NULL;
    size_t size = 0;
    char* fullpath = to_owned(pathname);

    stack_item_t current_dir = {fullpath, nlevel};
    push(&stack, &size, current_dir);

    while (size) {
        pop(&stack, &size, &current_dir);
        char *file;
        if ((file = strrchr(current_dir.dir, '/')))
            myfunc(file + 1, current_dir.nlevel);
        else
            myfunc(current_dir.dir, current_dir.nlevel);

        if ((dp = opendir(current_dir.dir)) != NULL)
            while ((dirp = readdir(dp)) != NULL) {
                if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
                    process_entry(current_dir.dir, dirp->d_name, &stack, &size, current_dir.nlevel + 1);
                }
            }

        free(current_dir.dir);
        if (closedir(dp) < 0) {
            printf("Can't close directory %s\n", fullpath);
        }
    }
    if (stack)
        free(stack);

    return 0;
}

int process_entry(char* cur_path, char* cur_file, stack_item_t **stack, size_t *stack_size, int nlevel) {
    struct stat statbuf;
    char* fullpath = concat(cur_path, cur_file);

    if (lstat(fullpath, &statbuf) < 0) 
        return -1;
    else if (S_ISDIR(statbuf.st_mode) != 0) {
        stack_item_t new_elem = { fullpath, nlevel };
        push(stack, stack_size, new_elem);
        return 0;
    } else {
        free(fullpath);
        return myfunc(cur_file, nlevel);
    }
}


