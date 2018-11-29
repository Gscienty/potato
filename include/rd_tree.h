#ifndef _POTATO_PAXOS_RD_TREE_
#define _POTATO_PAXOS_RD_TREE_

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
} pott_rd_tree_node;

struct pott_rd_tree {
    struct pott_rd_tree_node *root;
    int node_count;

    void (*dtor_key_cb)(void *);
    void (*dtor_data_cb)(void *);
    int (*key_cmp_cb)(void *, void *);

    struct pott_rd_tree_node nil;
} pott_rd_tree;

struct pott_rd_tree *
pott_rdtree_create(void (*dtor_key_cb)(void *),
                   void (*dtor_data_cb)(void *),
                   int (*key_cmp_cb)(void *, void *));
int pott_rdtree_insert(struct pott_rd_tree *tree, void *key, void *val);
struct pott_rd_tree_node *pott_rdtree_find(struct pott_rd_tree *tree, void *key);
int pott_rdtree_delete(struct pott_rd_tree *tree, struct pott_rd_tree_node *node);

#endif
