#include "tournament_tree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// members
typedef enum {
    TOURNAMENT_TREE_INNER_NODE,
    TOURNAMENT_TREE_LEAF_NODE,
} TournamentTreeNodeKind;

struct TournamentTreeBaseNode {
    TournamentTreeNodeKind kind;
    struct TournamentTreeInnerNode *parent;
    uint32_t height;
};

struct TournamentTreeInnerNode {
    // base
    TournamentTreeNodeKind kind;
    struct TournamentTreeInnerNode *parent;
    uint32_t height;
    // inner
    struct TournamentTreeLeafNode *leaf;
    union TournamentTreeNode *left_child;
    union TournamentTreeNode *right_child;
};

struct TournamentTreeLeafNode {
    // base
    TournamentTreeNodeKind kind;
    struct TournamentTreeInnerNode *parent;
    uint32_t height;
    // leaf
    int32_t key;
    void* value;
    bool owns_value;
    union TournamentTreeNode *node;
};

typedef union TournamentTreeNode {
    struct TournamentTreeBaseNode base;
    struct TournamentTreeInnerNode inner;
    struct TournamentTreeLeafNode leaf;
} TournamentTreeNode;

struct TournamentTree {
    TournamentTreeNode *root;
};

// private members
void _tournament_tree_delete_node(TournamentTreeNode *node);
struct TournamentTreeLeafNode *_tournament_tree_get_leaf(TournamentTreeNode *node);
void _tournament_tree_grow(TournamentTree *tree, uint32_t count);
void _tournament_tree_print_node(TournamentTreeNode *node, uint32_t generation);

// public functions
TournamentTree *tournament_tree_new(int32_t k, void* v, bool take_ownership) {
    struct TournamentTreeLeafNode *leaf = malloc(sizeof(struct TournamentTreeLeafNode));
    leaf->kind = TOURNAMENT_TREE_LEAF_NODE;
    leaf->parent = NULL;
    leaf->height = 1;
    leaf->key = k;
    leaf->value = v;
    leaf->owns_value = take_ownership;
    leaf->node = (TournamentTreeNode *)leaf;

    TournamentTree *tree = malloc(sizeof(TournamentTree));
    tree->root = (TournamentTreeNode *)leaf;

    return tree;
}

void tournament_tree_delete(TournamentTree *ds) {
    if (!ds) {
        return;
    }

    _tournament_tree_delete_node(ds->root);
    free(ds);
}

TournamentTreeLeafNode *tournament_tree_get_min(TournamentTree *t) {
    return _tournament_tree_get_leaf(t->root);
}

TournamentTree *tournament_tree_link(TournamentTree *t1, TournamentTree *t2) {
    uint32_t h1 = t1->root->base.height;
    uint32_t h2 = t2->root->base.height;
    if (h1 < h2) {
        _tournament_tree_grow(t1, h2 - h1);
    } else if (h2 < h1) {
        _tournament_tree_grow(t2, h1 - h2);
    }

    struct TournamentTreeInnerNode *root = malloc(sizeof(struct TournamentTreeInnerNode));
    root->kind = TOURNAMENT_TREE_INNER_NODE;
    root->parent = NULL;
    root->height = t1->root->base.height + 1;
    root->left_child = t1->root;
    root->right_child = t2->root;

    struct TournamentTreeLeafNode *lleaf = _tournament_tree_get_leaf(t1->root);
    struct TournamentTreeLeafNode *rleaf = _tournament_tree_get_leaf(t2->root);

    if (lleaf->key < rleaf->key) {
        root->leaf = lleaf;
        lleaf->node = (TournamentTreeNode *)root;
    } else {
        root->leaf = rleaf;
        rleaf->node = (TournamentTreeNode *)root;
    }

    t1->root->base.parent = root;
    t2->root->base.parent = root;

    free(t1);
    free(t2);

    TournamentTree *t = malloc(sizeof(TournamentTree));
    t->root = (TournamentTreeNode *)root;

    return t;
}

TournamentTree *tournament_tree_cut(TournamentTree *t, TournamentTreeLeafNode* n){
    TournamentTreeNode *root1 = t->root;
    TournamentTreeNode *root2 = n->node;

    if (root1 == root2) {
        return NULL;
    }

    struct TournamentTreeInnerNode *parent = root2->base.parent;

    if (parent->left_child == root2) {
        parent->left_child = NULL;
    } else {
        parent->right_child = NULL;
    }

    root2->base.parent = NULL;

    TournamentTree *t2 = malloc(sizeof(TournamentTree));
    t2->root = root2;

    return t2;
}

void tournament_tree_print(TournamentTree *t) {
    _tournament_tree_print_node(t->root, 0);
}

// private functions
void _tournament_tree_delete_node(TournamentTreeNode *node) {
    if (!node) {
        return;
    }

    switch (node->base.kind) {
    case TOURNAMENT_TREE_INNER_NODE:
        struct TournamentTreeInnerNode inner = node->inner;
        _tournament_tree_delete_node(inner.left_child);
        _tournament_tree_delete_node(inner.right_child);
        break;
    case TOURNAMENT_TREE_LEAF_NODE:
        struct TournamentTreeLeafNode leaf = node->leaf;

        if (leaf.owns_value) {
            free(leaf.value);
        }
        break;
    }

    free(node);
}

struct TournamentTreeLeafNode *_tournament_tree_get_leaf(TournamentTreeNode *node) {
    if (node->base.kind == TOURNAMENT_TREE_LEAF_NODE) {
        return (struct TournamentTreeLeafNode *)node;
    } else {
        return node->inner.leaf;
    }
}

void _tournament_tree_grow(TournamentTree *tree, uint32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        struct TournamentTreeInnerNode *n = malloc(sizeof(struct TournamentTreeInnerNode));
        n->kind = TOURNAMENT_TREE_INNER_NODE;
        n->parent = NULL;
        n->height = tree->root->base.height + 1;

        struct TournamentTreeLeafNode *leaf = _tournament_tree_get_leaf(tree->root);
        n->leaf = leaf;
        leaf->node = (TournamentTreeNode *)n;

        n->left_child = tree->root;
        n->right_child = NULL;
        tree->root->base.parent = n;
        tree->root = (TournamentTreeNode *)n;
    }
}

void _tournament_tree_print_node(TournamentTreeNode *node, uint32_t generation) {
    if (!node) {
        return;
    }

    for (uint32_t i = 0; i < generation; ++i) {
        printf("  ");
    }

    struct TournamentTreeLeafNode *leaf = _tournament_tree_get_leaf(node);
    printf("- %i, %i\n", leaf->key, leaf->value);

    if (node->base.kind == TOURNAMENT_TREE_INNER_NODE) {
        _tournament_tree_print_node(node->inner.left_child, generation + 1);
        _tournament_tree_print_node(node->inner.right_child, generation + 1);
    }
}
