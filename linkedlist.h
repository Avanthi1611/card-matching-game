#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node {
    char* data;
    struct Node* next;
    int hinted;
};

typedef struct Node Node;

void insert(Node** head, const char* value);
int search(Node* head, const char* value);
void mark_hint(Node** head, const char* value);
int is_hinted(Node* head, const char* value);
void clear_hint(Node** head, const char* value);
extern void free_list(Node* head);

#endif
