#ifndef _POTATO_PAXOS_SERVER_
#define _POTATO_PAXOS_SERVER_
#include "rd_tree.h"
#include <netinet/in.h>

struct pott_paxos_peer {
    unsigned long id;
    int fd;
};

struct pott_paxos_server_config {
    const char              *ip;
    const unsigned short    port;

    int (*key_cmp_cb)(void *, void *);
    void (*dtor_key_cb)(void *);
    void (*dtor_data_cb)(void *);
};

struct pott_paxos_server {
    int fd;
    pott_rd_tree *peers;
    struct sockaddr_in sockaddr;
};


struct pott_paxos_server *pott_create_server(const struct pott_paxos_server_config *config);

#endif
