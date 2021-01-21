/*************************************************************************
	> File Name: tcp_client.h
	> Author: jzh
	> Mail: 
	> Created Time: Tue 01 Dec 2020 06:21:40 PM CST
 ************************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

int socket_connect(char *ip, int port);
int socket_connect_timeout(char *ip, int port, long usec);

#endif
