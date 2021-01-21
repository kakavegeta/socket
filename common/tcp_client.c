/*************************************************************************
	> File Name: tcp_client.c
	> Author: jzh
	> Mail: 
	> Created Time: Tue 01 Dec 2020 06:23:58 PM CST
 ************************************************************************/

#include "head.h"


int socket_connect(char *ip, int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server ;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    return sockfd;
}

int socket_connect_timeout(char *ip, int port, long timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server ;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    make_nonblock(sockfd);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        int retval, error;
        retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);
        int len = sizeof(error);
        if (retval < 0) {
            close(sockfd);
            return -1;
        }
        else if (retval) {
            if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
                close(sockfd);
                return -1;
            }
            if (error) {
                close(sockfd);
                return -1;
            }
            printf("connected!\n");
        } else {
            printf("timeout!\n");
            close(sockfd);
            return -1;
        }
    }
     

    make_block(sockfd);
    return sockfd;
}
