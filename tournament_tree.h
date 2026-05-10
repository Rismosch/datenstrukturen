#ifndef TOURNAMENT_TREE_H
#define TOURNAMENT_TREE_H

#include <stdint.h>
typedef struct TournamentTree TournamentTree;

TournamentTree *tournament_tree_new(int32_t x[], uint32_t len);
void tournament_tree_delete(TournamentTree *ds);

#endif
