#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "stack.h"
#include "helpers.h"

char* to_owned(char* pathname);
int walk_dir(char* pathname);
int process_entry(char* cur_path, char* cur_file, stack_item_t **stack, size_t *stack_size, int nlevel);

