#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

// Insert a card into the linked list
void insert(Node** head, const char* value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = strdup(value);  // Duplicate the string for safety
    new_node->next = *head;
    new_node->hinted = 0;  // Initially, the card is not hinted
    *head = new_node;
}

// Search for a card in the linked list (return 1 if found, 0 if not)
int search(Node* head, const char* value) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            return 1;  // Found the card
        }
        temp = temp->next;
    }
    return 0;  // Card not found
}

// Mark a card as hinted
void mark_hint(Node** head, const char* value) {
    Node* temp = *head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            temp->hinted = 1;  // Mark as hinted
            return;
        }
        temp = temp->next;
    }
}

// Check if a card is hinted
int is_hinted(Node* head, const char* value) {
    Node* temp = head;
    while (temp) {
        if (strcmp(temp->data, value) == 0 && temp->hinted == 1) {
            return 1;  // Card is hinted
        }
        temp = temp->next;
    }
    return 0;  // Card is not hinted
}

// Clear the hint status of a card
void clear_hint(Node** head, const char* value) {
    Node* temp = *head;
    while (temp) {
        if (strcmp(temp->data, value) == 0) {
            temp->hinted = 0;  // Clear hint
            return;
        }
        temp = temp->next;
    }
}

// Free the linked list to prevent memory leaks
void free_list(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->data);  // Free the card data
        free(temp);  // Free the node itself
    }
}