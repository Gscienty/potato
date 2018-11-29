#include "debug.h"
#include "rd_tree.h"
#include <malloc.h>

static
struct pott_rd_tree_node *
__create_node(struct pott_rd_tree *tree, void *key, void *data) {
    struct pott_rd_tree_node *ret =
        (struct pott_rd_tree_node *) malloc(sizeof(struct pott_rd_tree_node));
    if (ret == NULL) {
        error("malloc failed");
        return NULL;
    }

    ret->tree = tree;
    ret->parent = &tree->nil;
    ret->left = &tree->nil;
    ret->right = &tree->nil;
    ret->color = RD_TREE_COLOR_RED;
    ret->data = data;
    ret->key = key;

    return ret;
}

static
void
__right_rotate(struct pott_rd_tree *tree, struct pott_rd_tree_node *x) {
    struct pott_rd_tree_node *y = x->left;
    x->left = y->right;
    if (y->right != &tree->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x == tree->root) {
        tree->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

static
void
__left_rotate(struct pott_rd_tree *tree, struct pott_rd_tree_node *x) {
    struct pott_rd_tree_node *y = x->right;
    x->right = y->left;
    if (y->left != &tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x == tree->root) {
        tree->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static
void __insert_fixup(struct pott_rd_tree *tree, struct pott_rd_tree_node *node) {
    while (node->parent->color == RD_TREE_COLOR_RED) {
        if (node->parent == node->parent->parent->left) {
            struct pott_rd_tree_node *y = node->parent->parent->right;
            if (y->color == RD_TREE_COLOR_RED) {
                node->parent->color = RD_TREE_COLOR_BLACK;
                y->color = RD_TREE_COLOR_BLACK;
                node->parent->parent->color = RD_TREE_COLOR_RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    __left_rotate(tree, node);
                }
                node->parent->color = RD_TREE_COLOR_BLACK;
                node->parent->parent->color = RD_TREE_COLOR_RED;
                __right_rotate(tree, node->parent->parent);
            }
        }
        else {
            struct pott_rd_tree_node *y = node->parent->parent->left;
            if (y->color == RD_TREE_COLOR_RED) {
                node->parent->color = RD_TREE_COLOR_BLACK;
                y->color = RD_TREE_COLOR_BLACK;
                node->parent->parent->color = RD_TREE_COLOR_RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    __right_rotate(tree, node);
                }
                node->parent->color = RD_TREE_COLOR_BLACK;
                node->parent->parent->color = RD_TREE_COLOR_RED;
                __left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = RD_TREE_COLOR_BLACK;
}

static
void __transplant(struct pott_rd_tree *tree,
                  struct pott_rd_tree_node *u,
                  struct pott_rd_tree_node *v) {
    if (u->parent == &tree->nil) {
        tree->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

static
void __delete_fixup(struct pott_rd_tree *tree, struct pott_rd_tree_node *x) {
    while (x != tree->root && x->color == RD_TREE_COLOR_BLACK) {
        if (x == x->parent->left) {
            struct pott_rd_tree_node *w = x->parent->right;
            if (w->color == RD_TREE_COLOR_RED) {
                w->color = RD_TREE_COLOR_BLACK;
                x->parent->color = RD_TREE_COLOR_RED;
                __left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RD_TREE_COLOR_BLACK && w->right->color == RD_TREE_COLOR_BLACK) {
                w->color = RD_TREE_COLOR_RED;
                x = x->parent;
            }
            else {
                if (w->right->color == RD_TREE_COLOR_BLACK) {
                    w->left->color = RD_TREE_COLOR_BLACK;
                    w->color = RD_TREE_COLOR_RED;
                    __right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RD_TREE_COLOR_BLACK;
                w->right->color = RD_TREE_COLOR_BLACK;
                __left_rotate(tree, x->parent);
                x = tree->root;
            }
        }
        else {
            struct pott_rd_tree_node *w = x->parent->left;
            if (w->color == RD_TREE_COLOR_RED) {
                w->color = RD_TREE_COLOR_BLACK;
                x->parent->color = RD_TREE_COLOR_RED;
                __right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == RD_TREE_COLOR_BLACK && w->right->color == RD_TREE_COLOR_BLACK) {
                w->color = RD_TREE_COLOR_RED;
                x = x->parent;
            }
            else {
                if (w->left->color == RD_TREE_COLOR_BLACK) {
                    w->right->color = RD_TREE_COLOR_BLACK;
                    w->color = RD_TREE_COLOR_RED;
                    __left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RD_TREE_COLOR_BLACK;
                x->left->color = RD_TREE_COLOR_BLACK;
                __right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RD_TREE_COLOR_BLACK;
}

static
struct pott_rd_tree_node * __minimum(struct pott_rd_tree *tree, struct pott_rd_tree_node *node) {
    // TODO assert
    while (node != &tree->nil) {
        if (node->left == &tree->nil) {
            return node;
        }
        node = node->left;
    }

    return &tree->nil;
}

struct pott_rd_tree *
pott_rdtree_create(void (*dtor_key_cb)(void *), 
                   void (*dtor_data_cb)(void *),
                   int (*key_cmp_cb)(void *, void *)) {
    struct pott_rd_tree *ret =
        (struct pott_rd_tree *) malloc(sizeof(struct pott_rd_tree));
    if (ret == NULL) {
        error("malloc failed");
        return NULL;
    }

    ret->root = &ret->nil;
    ret->node_count = 0;
    ret->dtor_key_cb = dtor_key_cb;
    ret->dtor_data_cb = dtor_data_cb;
    ret->key_cmp_cb = key_cmp_cb;

    ret->nil.color = RD_TREE_COLOR_BLACK;
    ret->nil.parent = &ret->nil;
    ret->nil.left = &ret->nil;
    ret->nil.right = &ret->nil;
    ret->nil.data = NULL;
    ret->nil.key = NULL;
    return ret;
}

int pott_rdtree_insert(struct pott_rd_tree *tree, void *key, void *data) {
    if (tree == NULL) {
        warn("tree is null");
        return -1;
    }
    if (tree->key_cmp_cb == NULL) {
        error("key cmp callback is null");
        return -2;
    }
    struct pott_rd_tree_node *node = __create_node(tree, key, data);
    if (node == NULL) {
        error("cannot create tree node");
        return -3;
    }

    tree->node_count++;
    if (tree->root == &tree->nil) {
        tree->root = node;
        __insert_fixup(tree, node);
        return 0;
    }

    struct pott_rd_tree_node *itr = tree->root;
    while (itr != &tree->nil) {
        int retcmp = tree->key_cmp_cb(node->key, itr->key);
        if (retcmp == 0) {
            // conflict
            warn("conflict");
            free(node);
            return -2;
        }
        else if (retcmp == -1) {
            if (itr->left != &tree->nil) {
                itr = itr->left;
            }
            else {
                itr->left = node;
                node->parent = itr;
                break;
            }
        }
        else if (retcmp == 1) {
            if (itr->right != &tree->nil) {
                itr = itr->right;
            }
            else {
                itr->right = node;
                node->parent = itr;
                break;
            }
        }
        else {
            // compare error
            warn("compare error, cmpret %d", retcmp);
            free(node);
            return -3;
        }
    }

    __insert_fixup(tree, node);

    return 0;
}

struct pott_rd_tree_node *pott_rdtree_find(struct pott_rd_tree *tree, void *key) {
    struct pott_rd_tree_node *itr = tree->root;
    while (itr != &tree->nil) {
        int retcmp = tree->key_cmp_cb(key, itr->key);
        if (retcmp == 0) {
            return itr;
        }
        else if (retcmp == -1) {
            itr = itr->left;
        }
        else if (retcmp == 1) {
            itr = itr->right;
        }
        else {
            return &tree->nil;
        }
    }

    return &tree->nil;
}

int pott_rdtree_delete(struct pott_rd_tree *tree, struct pott_rd_tree_node *node) {
    if (tree == NULL) {
        warn("tree is null");
        return -1;
    }
    if (node == NULL || node == &tree->nil) {
        warn("node is null");
        return -2;
    }
    if (tree != node->tree) {
        warn("this node is not belong with this tree");
    }

    struct pott_rd_tree_node *x;
    struct pott_rd_tree_node *y = node;
    int y_origin_color = y->color;
    if (node->left == &tree->nil) {
        x = node->right;
        __transplant(tree, node, node->right);
    }
    else if (node->right == &tree->nil) {
        x = node->left;
        __transplant(tree, node, node->left);
    }
    else {
        y = __minimum(tree, node->right);
        y_origin_color = y->color;
        x = y->right;
        if (y->parent == node) {
            x->parent = y;
        }
        else {
            __transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        __transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (y_origin_color == RD_TREE_COLOR_BLACK) {
        __delete_fixup(tree, x);
    }

    if (tree->dtor_key_cb != NULL) {
        tree->dtor_key_cb(node->key);
    }
    if (tree->dtor_data_cb != NULL) {
        tree->dtor_data_cb(node->data);
    }

    return 0;
}
