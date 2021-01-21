/*************************************************************************
	> File Name: udp_client.c
	> Author: akira
	> Mail: 
	> Created Time: Fri 15 Jan 2021 10:18:38 AM EST
 ************************************************************************/

#include "head.h"

int socket_udp() {
    int sockfd;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    return sockfd;
}

