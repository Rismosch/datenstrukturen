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
    TournamentTree *ds;

    int values[6] = {7, 4, 9, 10, 12, 0};

    ds = tournament_tree_new(values, 6);

    tournament_tree_delete(ds);
}

int main() {
    test_tournament_tree();
    return 0;
}

