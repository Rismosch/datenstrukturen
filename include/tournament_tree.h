#ifndef TOURNAMENT_TREE_H
#define TOURNAMENT_TREE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct TournamentTree TournamentTree;
typedef struct TournamentTreeLeafNode TournamentTreeLeafNode;

TournamentTree *tournament_tree_new(int32_t k, void* v, bool take_ownership);
void tournament_tree_delete(TournamentTree *t);

TournamentTreeLeafNode *tournament_tree_get_min(TournamentTree *t);
TournamentTree *tournament_tree_link(TournamentTree *t1, TournamentTree *t2);
TournamentTree *tournament_tree_cut(TournamentTree *t, TournamentTreeLeafNode* n);

void tournament_tree_print(TournamentTree *t);

#endif
