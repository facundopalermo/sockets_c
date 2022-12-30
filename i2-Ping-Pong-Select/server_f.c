#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "headers/server_f.h"

void setConfig(Server *server, int socket_domain, int socket_type, int port, int flag){

    int yes=1;

    server->socket = socket(socket_domain, socket_type, 0);
    server->port = port;

    memset(&server->addr, '0', sizeof(server->addr));

    server->addr.sin_family = socket_domain;
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr.sin_port = htons(server->port);
    
    bind(server->socket, (struct sockaddr*)&server->addr, sizeof(server->addr));
    
    listen(server->socket, BACKLOG);

    if(flag){
        setsockopt(server->socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
        fcntl(server->socket, F_SETFL, O_NONBLOCK);

        FD_ZERO(&server->fdSet.readfds);
        FD_SET(0, &server->fdSet.readfds);
        FD_SET(server->socket, &server->fdSet.readfds);
    }

}

int serverSelect(Server *server, int *fdmax) {

    int ret = 0;
    int fd = 0;
    char *chunk = NULL;
    char *pong = "PONG";
    char *noentendo = "NO ENTENDO";
    ssize_t readed;

    struct sockaddr sockaddClient;
    socklen_t sockaddClientLength;
    int socketNewCx;

    chunk = malloc(CHUNK_SIZE);
    memset(chunk, 0, CHUNK_SIZE);

    ret = select( (*fdmax) + 1, &server->fdSet.tempreadfds, NULL, NULL, NULL);

    if(ret == -1) {
        perror("select");
        return -1;
    }

    for(fd = 0; fd <= (*fdmax); fd++) {
            
        if(FD_ISSET(fd, &server->fdSet.tempreadfds)) {
            
            printf("Evento en descriptor: %d\n", fd);
            
            if(fd == 0) {
                memset(chunk, 0, CHUNK_SIZE);
                readed = read(0, chunk, CHUNK_SIZE);
                printf("Entrada en consola - %ld bytes leidos: %s\n", readed, chunk);
                continue;
            }
            
            if(fd == server->socket) {
                
                socketNewCx = accept(server->socket, &sockaddClient, &sockaddClientLength);
                FD_SET(socketNewCx, &server->fdSet.readfds);
                
                if(socketNewCx > (*fdmax)) {
                    (*fdmax) = socketNewCx;
                }
            } else {
                
                memset(chunk, 0, CHUNK_SIZE);
                readed = recv(fd, chunk, CHUNK_SIZE, 0);
            
                if(readed < 1) {
                    FD_CLR(fd, &server->fdSet.readfds);
                    printf("Conexion finalizada en descriptor: %d\n", fd);
                    continue;
                } else {
                    printf("socket %ld bytes leidos: %s\n", readed, chunk);

                    if(strcmp(chunk,"PING") == 0) {
                        send(fd, pong, strlen(pong), 0);
                    } else {
                        send(fd, noentendo, strlen(noentendo), 0);
                    }
                    
                    memset(chunk, 0, CHUNK_SIZE);
                }
            }
            
        }
    }
    printf("\n");
    free(chunk);
    return 0;
}


    