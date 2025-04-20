#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node {
    char* data;
    struct Node* next;
};

typedef struct Node Node;

void insert(Node** head, char* value);
int search(Node* head, char* value);

#endif