#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>
#include "linkedlist.h"
#include "stack.h"

#define TOTAL_CARDS 16

char* cards[TOTAL_CARDS];
int revealed[TOTAL_CARDS];
Stack undo_stack;
Node* matched_cards = NULL;

void strip_pair_suffix(char *filename) {
    char *png_pos = strstr(filename, ".png");
    if (png_pos) {
        *png_pos = '\0';
    }

    char *pair_pos = strstr(filename, "_pair");
    if (pair_pos) {
        *pair_pos = '\0';
    }
}

void get_filename(const char *path, char *filename) {
    const char *last_slash = strrchr(path, '/');
    if (last_slash) {
        strcpy(filename, last_slash + 1);
    } else {
        strcpy(filename, path);
    }
}

int is_matching_pair(const char* card1, const char* card2) {
    char base1[256], base2[256];

    get_filename(card1, base1);
    get_filename(card2, base2);

    strip_pair_suffix(base1);
    strip_pair_suffix(base2);

    return strcmp(base1, base2) == 0;
}

EMSCRIPTEN_KEEPALIVE
int is_matched(int index) {
    if (index < 0 || index >= TOTAL_CARDS) return 0;
    return search(matched_cards, cards[index]);
}

EMSCRIPTEN_KEEPALIVE
void flip_card(int index) {
    if (revealed[index] || is_matched(index)) return;
    revealed[index] = 1;
    push(&undo_stack, index);
}

EMSCRIPTEN_KEEPALIVE
int check_match(int idx1, int idx2) {
    if (idx1 == idx2) return 0;
    if (is_matched(idx1) || is_matched(idx2)) return 0;

    if (is_matching_pair(cards[idx1], cards[idx2])) {
        insert(&matched_cards, strdup(cards[idx1]));
        insert(&matched_cards, strdup(cards[idx2]));
        printf("Match found! Card %d and Card %d\n", idx1, idx2);
        return 1;
    }
    return 0;
}

EMSCRIPTEN_KEEPALIVE
int get_match_count() {
    int count = 0;
    Node* temp = matched_cards;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count / 2;
}

void shuffle_cards() {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        int r = rand() % TOTAL_CARDS;
        char* temp = cards[i];
        cards[i] = cards[r];
        cards[r] = temp;
    }
}

EMSCRIPTEN_KEEPALIVE
EMSCRIPTEN_KEEPALIVE
void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

void init_game() {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        revealed[i] = 0;
    }
    free_list(matched_cards);  // Clear old matches
    matched_cards = NULL;
    init_stack(&undo_stack);
}



EMSCRIPTEN_KEEPALIVE
void set_card(int index, const char* path) {
    if (index >= 0 && index < TOTAL_CARDS) {
        cards[index] = strdup(path); 
    }
}

EMSCRIPTEN_KEEPALIVE
void undo_flip() {
    if (!is_empty(&undo_stack)) {
        int last = pop(&undo_stack);
        if (!is_matched(last)) {
            revealed[last] = 0;
        }
    }
}