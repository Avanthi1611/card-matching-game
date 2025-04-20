#ifndef STACK_H
#define STACK_H

#define MAX 100

typedef struct {
    int data[MAX];
    int top;
} Stack;

void init_stack(Stack* s);
void push(Stack* s, int val);
int pop(Stack* s);
int is_empty(Stack* s);

#endif