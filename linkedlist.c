#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void insert(Node** head, const char* value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = strdup(value);
    new_node->next = *head;
    new_node->hinted = 0;
    *head = new_node;
}

int search(Node* head, const char* value) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void mark_hint(Node** head, const char* value) {
    Node* temp = *head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            temp->hinted = 1;
            return;
        }
        temp = temp->next;
    }
}

int is_hinted(Node* head, const char* value) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->data, value) == 0 && temp->hinted == 1) {
            return 1; 
        }
        temp = temp->next;
    }
    return 0;
}

void clear_hint(Node** head, const char* value) {
    Node* temp = *head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            temp->hinted = 0;
            return;
        }
        temp = temp->next;
    }
}

void free_list(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}