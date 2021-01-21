/*************************************************************************
	> File Name: common.c
	> Author: akira
	> Mail: 
	> Created Time: Fri 15 Jan 2021 10:41:19 AM EST
 ************************************************************************/

#include "head.h"

int make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}

int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}

char *get_value(const char *path, const char *key) {
    FILE *fp = NULL;
    char *line = NULL, *sub = NULL;
    char *ans = (char *)malloc(sizeof(char) * 512);
    size_t nread, len;
    if (path == NULL || key == NULL) {
        fprintf(stderr, "Error in arguments!\n");
        errno = EINVAL;
        //return NULL;
        exit(1);
    } 

    if ((fp = fopen(path, "r")) == NULL) {
        //return NULL;
        exit(1);
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) {
            continue;
        } 

        if (line[strlen(key)] == '=' && sub == line) {
            strcpy(ans, line + strlen(key) + 1);
            ans[strlen(ans)-1] = '\0';
        }
    }
    free(line);
    free(sub);
    fclose(fp);
    return ans;
}

