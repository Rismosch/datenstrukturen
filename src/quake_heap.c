#include "quake_heap.h"

#include <stdint.h>
#include <stdlib.h>

#include "tournament_tree.h"

#define BUF_LEN 1<<8

struct QuakeHeap {
    TournamentTree** T;
    uint32_t* n;
};

QuakeHeap* quake_heap_new() {
    QuakeHeap* q = malloc(sizeof(QuakeHeap));
    q->T = malloc(sizeof(TournamentTree*) * BUF_LEN);
    q->n = malloc(sizeof(uint32_t) * BUF_LEN);

    for (int i = 0; i < BUF_LEN; ++i) {
        q->T[i] = NULL;
        q->n[i] = 0;
    }

    return q;
}

void quake_heap_delete(QuakeHeap* q) {
    for (int i = 0; i < BUF_LEN; ++i) {
        tournament_tree_delete(q->T[i]);
    }

    free(q->T);
    free(q->n);
    free(q);
}

