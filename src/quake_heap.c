#include "quake_heap.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tournament_tree.h"

#define INITIAL_BUF_LEN 4

struct QuakeHeap {
    TournamentTree** T;
    size_t T_capacity;
    size_t T_len;
};

QuakeHeap* quake_heap_new() {
    QuakeHeap* q = malloc(sizeof(QuakeHeap));

    q->T_capacity = INITIAL_BUF_LEN;
    q->T_len = 0;
    q->T = malloc(sizeof(TournamentTree*) * q->T_capacity);

    return q;
}

void quake_heap_delete(QuakeHeap* q) {
    for (size_t i = 0; i < q->T_len; ++i) {
        tournament_tree_delete(q->T[i]);
    }

    free(q->T);
    free(q);
}

TournamentTreeLeafNode* quake_heap_insert(QuakeHeap* q, uint32_t k, void* x, bool take_ownership) {
    // list is full, resize
    if (q->T_len == q->T_capacity) {
        size_t T_capacity_new = q->T_capacity * 2;
        TournamentTree** T_new = malloc(sizeof(TournamentTree*) * T_capacity_new);

        memcpy(T_new, q->T, sizeof(TournamentTree*) * q->T_capacity);
        free(q->T);

        q->T_capacity = T_capacity_new;
        q->T = T_new;
    }

    // add new tree
    TournamentTree* t = tournament_tree_new(k, x, take_ownership);
    q->T[q->T_len] = t;
    q->T_len += 1;

    // return leaf
    TournamentTreeLeafNode* leaf = tournament_tree_get_min(t);
    return leaf;
}

void quake_heap_decrease_key(TournamentTreeLeafNode* x, uint32_t k) {

}

void quake_heap_print(QuakeHeap* q) {
    for (size_t i = 0; i < q->T_len; ++i) {
        printf("\n%i\n", i);
        tournament_tree_print(q->T[i]);
    }
}
