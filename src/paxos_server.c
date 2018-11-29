#include "debug.h"
#include "paxos_server.h"
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// ctor paxos server by paxos server config
// param "config": paxos server config
// result: paxos server
static
struct pott_paxos_server *
__ctor_paxos_server(const struct pott_paxos_server_config *config) {
    info("build paxos server");
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);

    // ctor sockaddr_in
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config->port);
    addr.sin_addr.s_addr = inet_addr(config->ip);

    // malloc paxos server
    struct pott_paxos_server *svr = (struct pott_paxos_server *) malloc(sizeof(struct pott_paxos_server));
    if (svr == NULL) {
        error("malloc failed");
        return NULL;
    }

    // init paxos server
    svr->fd = sfd;
    svr->sockaddr = addr;
    svr->peers = pott_rdtree_create(config->dtor_key_cb, config->dtor_data_cb, config->key_cmp_cb);

    return svr;
}

// init paxos server
// param "svr": paxos server
// result:  
// | result code | status     |
// | -           | -          |
// | 0           | success    |
// | -1          | failed     |
// | -2          | bind error |
static
int
__init_paxos_server(const struct pott_paxos_server *svr) {
    if (svr == NULL) {
        error("svr is null");
        return -1;
    }
    int ret;

    ret = bind(svr->fd, (struct sockaddr *) &svr->sockaddr, sizeof(struct sockaddr_in));
    if (ret != 0) {
        error("svr cannot bind fd")
        return -2;
    }

    return 0;
}

// dtor paxos server
// param "svr": paxos server
// result:
// | result code | status      |
// | -           | -           |
// | 0           | success     |
// | -1          | failed      |
// | -2          | svr is null |
static
int
__dtor_paxos_server(struct pott_paxos_server *svr) {
    int retval;

    if (svr == NULL) { return -2; }
    retval = close(svr->fd);
    free(svr);

    return retval;
}

struct pott_paxos_server *
pott_create_server(const struct pott_paxos_server_config *config) {
    // ctor paxos server
    struct pott_paxos_server *svr = __ctor_paxos_server(config);
    if (svr == NULL) {
        return NULL;
    }

    // init paxos server
    if (__init_paxos_server(svr) == 0) {
        __dtor_paxos_server(svr);
        return NULL;
    }

    return svr;
}
