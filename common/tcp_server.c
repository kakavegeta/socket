/*************************************************************************
	> File Name: tcp_server.c
	> Author: akira
	> Mail: 
	> Created Time: Fri 04 Dec 2020 07:27:18 AM EST
 ************************************************************************/

#include "head.h"


int socket_create(int port) {
    int server_listen;
    if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    if (listen(server_listen, 20) < 0) {
        return -1;
    }
    return server_listen;

}
