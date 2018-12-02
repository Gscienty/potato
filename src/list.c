#include "list.h"
#include "debug.h"
#include <malloc.h>

static
struct pott_list * __create(void) {
    struct pott_list *ret = (struct pott_list *) malloc(sizeof(struct pott_list));

    if (ret == NULL) {
        error("malloc failed");
        return NULL;
    }

    return ret;
}

static
int __init(struct pott_list *lst) {
    if (lst == NULL) {
        warn("lst is null");
        return -1;
    }

    lst->node_count = 0;
    lst->head.data = NULL;
    lst->head.next = &lst->head;
    lst->head.prev = &lst->head;
    lst->head.list = lst;

    return 0;
}

static
struct pott_list_node *__create_node(struct pott_list *lst, void *data) {
    if (lst == NULL) {
        error("lst is null");
        return NULL;
    }

    struct pott_list_node *ret = (struct pott_list_node *) malloc(sizeof(struct pott_list_node));
    if (ret == NULL) {
        error("malloc failed");
        return NULL;
    }

    ret->data = data;
    ret->list = lst;
    ret->next = &lst->head;
    ret->prev = &lst->head;

    return ret;
}

struct pott_list *pott_list_create(void) {
    struct pott_list *ret = __create();
    if (ret == NULL) {
        error("create failed");
        return NULL;
    }

    if (__init(ret) != 0) {
        warn("init failed");
        free(ret);
        return NULL;
    }

    return ret;
}

struct pott_list_node *pott_list_front(struct pott_list *lst) {
    if (lst == NULL) {
        warn("lst is null");
        return NULL;
    }

    if (lst->head.next == &lst->head) {
        warn("lst is empty");
        return NULL;
    }

    return lst->head.next;
}

struct pott_list_node *pott_list_tail(struct pott_list *lst) {
    if (lst == NULL) {
        warn("lst is null");
        return NULL;
    }

    if (lst->head.prev == &lst->head) {
        warn("lst is empty");
        return NULL;
    }

    return lst->head.prev;
}

int pott_list_push_back(struct pott_list *lst, void *data) {
    if (lst == NULL) {
        error("lst is null");
        return -1;
    }

    struct pott_list_node *node = __create_node(lst, data);

    lst->head.prev->next = node;
    node->prev = lst->head.prev;

    lst->head.prev = node;

    return 0;
}

int pott_list_push_front(struct pott_list *lst, void *data) {
    if (lst == NULL) {
        error("lst is null");
        return -1;
    }

    struct pott_list_node *node = __create_node(lst, data);

    lst->head.next->prev = node;
    node->next = lst->head.next;

    lst->head.next = node;

    return 0;
}

void *pott_list_pop_back(struct pott_list *lst) {
    if (lst == NULL) {
        error("lst is null");
        return NULL;
    }
    if (lst->head.prev == &lst->head) {
        warn("lst is empty");
        return NULL;
    }
    
    void *ret = lst->head.prev->data;

    struct pott_list_node *node = lst->head.prev;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node);

    return ret;
}

void *pott_list_pop_front(struct pott_list *lst) {
    if (lst == NULL) {
        error("lst is null");
        return NULL;
    }
    if (lst->head.next == &lst->head) {
        warn("lst is empty");
        return NULL;
    }

    void *ret = lst->head.next->data;

    struct pott_list_node *node = lst->head.next;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node);

    return ret;
}

bool_t pott_list_empty(struct pott_list *lst) {
    if (lst == NULL) {
        error("lst is null");
        return 1;
    }

    return lst->head.next == &lst->head;
}
