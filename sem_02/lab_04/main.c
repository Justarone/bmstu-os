#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 10000

#define NO_ACCESS 1
#define BAD_ARGS 2

void print_cmdline(const int pid) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%d/cmdline", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, file);
    buf[len - 1] = 0;
    printf("\nCMDLINE:\n%s\n", buf);

    fclose(file);
}

void print_environ(const int pid) {
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "/proc/%d/environ", pid);
    FILE *file = fopen(pathToOpen, "r");

    int len;
    char buf[BUF_SIZE];
    printf("\nENVIRON:\n");
    while ((len = fread(buf, 1, BUF_SIZE, file)) > 0) {
        for (int i = 0; i < len; i++)
            if (!buf[i])
                buf[i] = '\n';
        buf[len - 1] = '\n';
        printf("%s", buf);
    }

    fclose(file);
}

void print_fd(const int pid) {
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "/proc/%d/fd/", pid);
    DIR *dir = opendir(pathToOpen);

    printf("\nFD:\n");

    struct dirent *readDir;
    char string[PATH_MAX];
    char path[BUF_SIZE] = {'\0'};
    while ((readDir = readdir(dir)) != NULL) {
        if ((strcmp(readDir->d_name, ".") != 0) && (strcmp(readDir->d_name, "..") != 0)) {
            sprintf(path, "%s%s", pathToOpen, readDir->d_name);
            readlink(path, string, PATH_MAX);
            printf("%s -> %s\n", readDir->d_name, string);
        }
    }

    closedir(dir);
}

void print_stat(const int pid) {
    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "/proc/%d/stat", pid);
    char buf[BUF_SIZE];

    FILE *file = fopen(pathToOpen, "r");
    fread(buf, 1, BUF_SIZE, file);
    char *token = strtok(buf, " ");

    printf("\nSTAT: \n");

    for (int i = 1; token != NULL; i++) {
        printf("%d. %s \n", i, token);
        token = strtok(NULL, " ");
    }

    fclose(file);
}

int get_pid(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Введите pid процесса для исследования!");
        exit(BAD_ARGS);
    }
    int pid = atoi(argv[1]);
    char check_dir[PATH_MAX];
    snprintf(check_dir, PATH_MAX, "/proc/%d", pid);
    if (!pid || access(check_dir, F_OK)) {
        printf("Директория для введенного pid не доступна!");
        exit(NO_ACCESS);
    }
    return pid;
}

int main(int argc, char *argv[])
{
    int pid = get_pid(argc, argv);
    print_cmdline(pid);
    print_environ(pid);
    print_fd(pid);
    print_stat(pid);
    return 0;
}
