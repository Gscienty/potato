#ifndef _POTATO_PAXOS_RD_TREE_
#define _POTATO_PAXOS_RD_TREE_

#include "type.h"

enum RD_TREE_COLOR {
    RD_TREE_COLOR_RED   = 1,
    RD_TREE_COLOR_BLACK = 0
};

struct pott_rd_tree;

struct pott_rd_tree_node {
    void *data;
    void *key;

    struct pott_rd_tree *tree;    
    struct pott_rd_tree_node *parent;
    struct pott_rd_tree_node *left;
    struct pott_rd_tree_node *right;

    int color;
};

struct pott_rd_tree {
    struct pott_rd_tree_node *root;
    int node_count;

    void (*dtor_key_cb)(void *);
    void (*dtor_data_cb)(void *);
    int (*key_cmp_cb)(void *, void *);

    struct pott_rd_tree_node nil;
};

struct pott_rd_tree *
pott_rdtree_create(void (*dtor_key_cb)(void *),
                   void (*dtor_data_cb)(void *),
                   int (*key_cmp_cb)(void *, void *));
int pott_rdtree_insert(struct pott_rd_tree *tree, void *key, void *val);
struct pott_rd_tree_node *pott_rdtree_find(struct pott_rd_tree *tree, void *key);
int pott_rdtree_delete(struct pott_rd_tree *tree, struct pott_rd_tree_node *node);
int pott_rdtree_clear(struct pott_rd_tree *tree);
bool_t pott_rdtree_empty(struct pott_rd_tree *tree);
void pott_rdtree_each(struct pott_rd_tree *tree, void (*cb)(const void *, void *));

#endif
