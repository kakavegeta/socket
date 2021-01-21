/*************************************************************************
	> File Name: udp_server.c
	> Author: akira
	> Mail: 
	> Created Time: Fri 15 Jan 2021 10:08:52 AM EST
 ************************************************************************/

#include "head.h"

int socket_create_upd(int port) {
    int listen;
    if ((listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    unsigned long opt = 1;
    setsockopt(listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    return listen;
}
