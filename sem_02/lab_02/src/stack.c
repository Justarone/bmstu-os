#include "stack.h"

void push(stack_item_t **stack, size_t *size, stack_item_t elem) {
    *stack = realloc(*stack, sizeof(stack_item_t) * (*size + 1));
    if (*stack == NULL) {
        printf("Memory error!");
        exit(1);
    }
    (*stack)[(*size)++] = elem;
}

void pop(stack_item_t **stack, size_t *size, stack_item_t *elem) {
    if (*size > 0) {
        *elem = (*stack)[--(*size)];
    } else {
        printf("Stack pop error!");
        exit(1);
    }
}
