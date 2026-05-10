#ifndef QUAKE_HEAP_H
#define QUAKE_HEAP_H

#include <stdint.h>

#include "tournament_tree.h"

typedef struct QuakeHeap QuakeHeap;

QuakeHeap* quake_heap_new();
void quake_heap_delete(QuakeHeap* q);

TournamentTreeLeafNode* quake_heap_insert(void* x, uint32_t k);

#endif
