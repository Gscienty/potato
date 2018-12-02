#ifndef _POTATO_LIST_
#define _POTATO_LIST_
#include "type.h"

struct pott_list;

struct pott_list_node {
    void *data;

    struct pott_list_node *next;
    struct pott_list_node *prev;
    struct pott_list *list;
};

struct pott_list {
    struct pott_list_node head;
    int node_count;
};

struct pott_list *pott_list_create(void);
struct pott_list_node *pott_list_front(struct pott_list *lst);
struct pott_list_node *pott_list_tail(struct pott_list *lst);
int pott_list_push_back(struct pott_list *lst, void *data);
int pott_list_push_front(struct pott_list *lst, void *data);
void *pott_list_pop_back(struct pott_list *lst);
void *pott_list_pop_front(struct pott_list *lst);
bool_t pott_list_empty(struct pott_list *lst);

#endif
