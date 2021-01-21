/*************************************************************************
	> File Name: common.h
	> Author: akira
	> Mail: 
	> Created Time: Fri 15 Jan 2021 10:40:38 AM EST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int fd);
int make_block(int fd);
char *get_value(const char *path, const char *key);

#endif
