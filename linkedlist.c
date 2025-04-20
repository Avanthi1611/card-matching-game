#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void insert(Node** head, char* value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = strdup(value);
    new_node->next = *head;
    *head = new_node;
}

int search(Node* head, char* value) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) return 1;
        temp = temp->next;
    }
    return 0;
}