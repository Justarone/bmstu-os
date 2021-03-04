#include <stdlib.h>
#include <stdio.h>

typedef struct stack_item_t {
    char* dir;
    int nlevel;
} stack_item_t;

void push(stack_item_t **stack, size_t *size, stack_item_t elem);
void pop(stack_item_t **stack, size_t *size, stack_item_t *elem);
