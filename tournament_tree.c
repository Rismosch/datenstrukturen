#include "tournament_tree.h"

#include <stdint.h>
#include <stdlib.h>

#include "ring_buffer.h"

// members
typedef enum {
    NODEKIND_INNER,
    NODEKIND_LEAF,
} NodeKind;

typedef struct BaseNode BaseNode;
typedef struct InnerNode InnerNode;
typedef struct LeafNode LeafNode;

struct BaseNode {
    NodeKind kind;
    InnerNode *parent;
};

struct InnerNode {
    // base
    NodeKind kind;
    InnerNode *parent;
    // inner
    LeafNode *leaf;
    union Node *left_child;
    union Node *right_child;
};

struct LeafNode {
    // base
    NodeKind kind;
    InnerNode *parent;
    // leaf
    int value;
    union Node *node;
};

typedef union Node {
    BaseNode base;
    InnerNode inner;
    LeafNode leaf;
} Node;

struct TournamentTree {
    Node *root;
};

void _tournament_tree_delete_node(Node *node);

// public functions
TournamentTree *tournament_tree_new(int32_t x[], uint32_t len) {
    if (!x || len == 0) {
        return NULL;
    }

    uint32_t i;
    RingBuffer *rb;
    LeafNode *leaf;

    rb = ring_buffer_new(len);

    // alloc leafs
    for (i = 0; i < len; ++i) {
        leaf = malloc(sizeof(LeafNode));
        leaf->kind = NODEKIND_LEAF;
        leaf->parent = NULL;
        leaf->value = x[i];
        leaf->node = (Node*)leaf;

        ring_buffer_enque(rb, leaf);
    }

    // build tree
    while (ring_buffer_len(rb) >= 2) {
        InnerNode *parent;
        Node *l;
        Node *r;

        (void)ring_buffer_deque(rb, (void **)&l);
        (void)ring_buffer_deque(rb, (void **)&r);

        // alloc parent
        parent = malloc(sizeof(InnerNode));
        parent->kind = NODEKIND_INNER;
        parent->left_child = l;
        parent->right_child = r;

        // assign parent
        l->base.parent = parent;
        r->base.parent = parent;

        // compare values
        LeafNode *l_leaf, *r_leaf;

        switch (l->base.kind) {
        case NODEKIND_INNER:
            l_leaf = l->inner.leaf;
            break;
        case NODEKIND_LEAF:
            l_leaf = &l->leaf;
            break;
        }

        switch (r->base.kind) {
        case NODEKIND_INNER:
            r_leaf = r->inner.leaf;
            break;
        case NODEKIND_LEAF:
            r_leaf = &r->leaf;
            break;
        }

        // assign leaf and node references
        if (l_leaf->value < r_leaf->value) {
            parent->leaf = l_leaf;
            l_leaf->node = (Node*)parent;
        } else {
            parent->leaf = r_leaf;
            r_leaf->node = (Node*)parent;
        }

        // push back to queue
        ring_buffer_enque(rb, parent);
    }

    // final tree setup
    TournamentTree* tree = malloc(sizeof(TournamentTree));
    ring_buffer_deque(rb, (void **)&tree->root);

    return tree;
}

void tournament_tree_delete(TournamentTree *ds) {
    if (!ds) {
        return;
    }

    _tournament_tree_delete_node(ds->root);
    free(ds);
}

void tournament_tree_print(TournamentTree *ds) {
    RingBuffer *rb = ring_buffer_new(1<<10);

    ring_buffer_enque(rb, ds->root);
}

// private functions
void _tournament_tree_delete_node(Node *node) {
    if (!node) {
        return;
    }

    if (node->base.kind == NODEKIND_INNER) {
        InnerNode inner = node->inner;
        _tournament_tree_delete_node(inner.left_child);
        _tournament_tree_delete_node(inner.right_child);
    }

    free(node);
}

