#ifndef QUAKE_HEAP_H
#define QUAKE_HEAP_H

#include <stdbool.h>
#include <stdint.h>

#include "tournament_tree.h"

typedef struct QuakeHeap QuakeHeap;

QuakeHeap* quake_heap_new();
void quake_heap_delete(QuakeHeap* q);

TournamentTreeLeafNode* quake_heap_insert(QuakeHeap* q, uint32_t k, void* x, bool take_ownership);
void quake_heap_decrease_key(TournamentTreeLeafNode* x, uint32_t k);

void quake_heap_print(QuakeHeap* q);


#endif
