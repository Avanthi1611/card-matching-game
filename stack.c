#include "stack.h"

void init_stack(Stack* s) {
    s->top = -1;
}

void push(Stack* s, int val) {
    if (s->top < MAX - 1) {
        s->data[++s->top] = val;
    }
}

int pop(Stack* s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    }
    return -1;
}

int is_empty(Stack* s) {
    return s->top == -1;
}