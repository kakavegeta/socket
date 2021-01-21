

#include "head.h"

#define MAX_CLIENTS 30

struct Client {
    char name[20];
    int sockfd;
};

int main(int argc, char **argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        exit(1);
    }

    int listenfd, sockfd, max_fd;

    struct Client *clients = (struct Client *)malloc(sizeof(struct Client)*MAX_CLIENTS);

    for (int i=0; i<MAX_CLIENTS; ++i) {
        memset(clients[i].name, 0, sizeof(clients[i].name));
        clients[i].sockfd = -1; 
    }

    int port = atoi(argv[1]);

    if ((listenfd = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }

    max_fd = listenfd;
    int ret;
    fd_set rfds;

    while (1) {

        FD_ZERO(&rfds);
        FD_SET(listenfd, &rfds);

        for (int i=0; i<MAX_CLIENTS; ++i) {
            if (listenfd == clients[i].sockfd) continue;
            if (clients[i].sockfd > 0) {
                if (clients[i].sockfd > max_fd) max_fd = clients[i].sockfd;
                FD_SET(clients[i].sockfd, &rfds);
            }
        }

        
        int nready = select(max_fd+1, &rfds, NULL, NULL, NULL);
        if (nready < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(listenfd, &rfds)) {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            if ((sockfd = accept(listenfd, (struct sockaddr *)&client, &len)) < 0) {
                perror("accept");
                exit(1);
            }
            printf("accept: %s %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            
            if (sockfd > MAX_CLIENTS) {
                fprintf(stderr, "too many clients\n");
                close(sockfd);
            } else {
                if (clients[sockfd].sockfd == -1) clients[sockfd].sockfd = sockfd;
            }
        } 

        // deal with clients' message
        for (int i=0; i<MAX_CLIENTS; ++i) {
            if (clients[i].sockfd == listenfd || clients[i].sockfd < 0) continue;
            if (FD_ISSET(clients[i].sockfd, &rfds)) {
                if (strlen(clients[i].name) == 0) {
                    // client connect to server, but say nothing and disconnect
                    ret = recv(clients[i].sockfd, clients[i].name, sizeof(clients[i].name), 0);
                    if (ret <= 0) {
                        close(clients[i].sockfd);
                        clients[i].sockfd = -1;
                        memset(clients[i].name, 0, sizeof(clients[i].name));
                        FD_CLR(clients[i].sockfd, &rfds);
                    }
                    printf("login: %s \n", clients[i].name);
                    continue;
                }
                char buff[1024] = {0};
                ret = recv(clients[i].sockfd, buff, sizeof(buff), 0);
                if (ret <= 0) {
                    close(clients[i].sockfd);
                    clients[i].sockfd = -1;
                    printf("logout: %s\n", clients[i].name);
                    memset(clients[i].name, 0, sizeof(clients[i].name));
                    FD_CLR(clients[i].sockfd, &rfds);
                }
                printf("%s : %s \n", clients[i].name, buff);
                send(clients[i].sockfd, buff, strlen(buff), 0);
            }
        }

    }
    return 0;
}
