

#include "common/head.h"

#define MAX_CLIENTS 30
#define MAX_EVENTS 5
struct Client {
    char name[20];
    int sockfd;
};


int main(int argc, char **argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        exit(1);
    }

    int nfds, listenfd, sockfd, epollfd;

    struct epoll_event ev, events[MAX_EVENTS];

    struct Client *clients = (struct Client *)malloc(sizeof(struct Client)*MAX_CLIENTS);

    for (int i=0; i<MAX_CLIENTS; ++i) {
        memset(clients[i].name, 0, sizeof(clients[i].name));
        clients[i].sockfd = -1; 
    }

    int port = atoi(argv[1]);

    if ((epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }

    if ((listenfd = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }

    clients[listenfd].sockfd = listenfd;
    strcpy(clients[listenfd].name, "Listener");
    ev.events = EPOLLIN;
    ev.data.ptr = &clients[listenfd];

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }

    while (1) {
        if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; ++i) {
            if (((struct Client *)events[i].data.ptr)->sockfd == listenfd) {
                if ((sockfd = accept(listenfd, NULL, NULL)) < 0) {
                    perror("accept");
                    exit(1);
                } 
                if (sockfd > MAX_CLIENTS) {
                    fprintf(stderr, "TOO MANY \n");
                    continue;
                }

                clients[sockfd].sockfd = sockfd;
                ev.data.ptr = &clients[sockfd];
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
                    perror("epoll_ctl");
                    exit(1);
                }
            } else {
                if (events[i].events & (EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {
                    char buff[1024];
                    int fd = events[i].data.ptr->sockfd;
                    if (strlen(events[i].data.ptr->name) == 0) {
                        int ret = recv(fd, events[i].data.ptr->name, sizeof(events[i].data.ptr->name) , 0); 
                        events[i].data.ptr->name[strlen(events[i].data.ptr->name)] = '\0';
                        if (ret <= 0) {
                            close(fd);
                            clients[fd].sockfd = -1;
                            memset(clients[fd].name, 0, sizeof(clients[fd].name));
                            epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                        }
                        continue;
                    }
                    int ret = recv(fd, buff, sizeof(buff), 0);
                    if (ret <= 0) {
                        printf("%s logout\n", clients[fd].name);
                        close(fd);
                        clients[fd].sockfd = -1;
                        memset(clients[fd].name, 0, sizeof(clients[fd].name));
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                    }
                    printf("%s %s\n", clients[fd].name, buff);

                }
            }
        } 
    }




    fd_set rfds;

    return 0;
}
