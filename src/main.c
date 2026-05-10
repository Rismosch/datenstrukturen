#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ring_buffer.h"
#include "tournament_tree.h"

void test_ring_buffer() {
    RingBuffer *ds = ring_buffer_new(3);
    bool success;

    success = ring_buffer_enque(ds, (void*)42);
    printf("success: %i, len: %i\n", success, ring_buffer_len(ds));
    ring_buffer_enque(ds, (void*)13);
    printf("success: %i, len: %i\n", success, ring_buffer_len(ds));
    ring_buffer_enque(ds, (void*)-12);
    printf("success: %i, len: %i\n", success, ring_buffer_len(ds));
    ring_buffer_enque(ds, (void*)8);
    printf("success: %i, len: %i\n", success, ring_buffer_len(ds));

    int x;
    success = ring_buffer_deque(ds, (void**)&x);
    printf("success: %i, len: %i, x: %i\n", success, ring_buffer_len(ds), x);
    success = ring_buffer_deque(ds, (void**)&x);
    printf("success: %i, len: %i, x: %i\n", success, ring_buffer_len(ds), x);
    success = ring_buffer_deque(ds, (void**)&x);
    printf("success: %i, len: %i, x: %i\n", success, ring_buffer_len(ds), x);
    success = ring_buffer_deque(ds, (void**)&x);
    printf("success: %i, len: %i, x: %i\n", success, ring_buffer_len(ds), x);

    ring_buffer_delete(ds);
}

void test_tournament_tree() {
    TournamentTree *t7 = tournament_tree_new(7);
    TournamentTree *t4 = tournament_tree_new(4);
    TournamentTree *t9 = tournament_tree_new(9);
    TournamentTree *t10 = tournament_tree_new(10);
    TournamentTree *t12 = tournament_tree_new(12);
    TournamentTree *t0 = tournament_tree_new(0);

    TournamentTreeLeafNode *n4 = tournament_tree_get_min(t4);

    t4 = tournament_tree_link(t4, t9);
    t10 = tournament_tree_link(t10, t12);
    t4 = tournament_tree_link(t4, t10);
    t0 = tournament_tree_link(t4, t0);
    t0 = tournament_tree_link(t7, t0);
    TournamentTree *t = t0;

    printf("tree:\n");
    tournament_tree_print(t);

    TournamentTree *part = tournament_tree_cut(t, n4);

    printf("tree:\n");
    tournament_tree_print(t);
    printf("part:\n");
    tournament_tree_print(part);

    tournament_tree_delete(t);
    tournament_tree_delete(part);
}

int main() {
    test_tournament_tree();
    return 0;
}

