#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

#include "headers/server_f.h"

int setConfig(Server *server, int socket_domain, int socket_type, int port, int flag){

    int yes = 1;
    int ret = 0;
    char c_port[1024];
    struct addrinfo hints;
    struct addrinfo *serverInfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family 	= socket_domain;
    hints.ai_socktype 	= socket_type;
    hints.ai_flags 		= AI_PASSIVE;

    sprintf(c_port, "%d", port);
    getaddrinfo(NULL, c_port, &hints, &serverInfo);

    if(serverInfo == NULL) {
        
        printf("Local ip address coudn't be obtained.\n");
        exit(EXIT_FAILURE);
    }
    
    if(ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        freeaddrinfo(serverInfo);
        return -1;
    }
    
    server->addr = serverInfo;
    server->socket = socket(server->addr->ai_family, server->addr->ai_socktype, server->addr->ai_protocol);
    server->port = port;
    
    bind(server->socket, server->addr->ai_addr, server->addr->ai_addrlen);
    listen(server->socket, BACKLOG);

    if(flag){
        setsockopt(server->socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
        fcntl(server->socket, F_SETFL, O_NONBLOCK);

        FD_ZERO(&server->fdSet.readfds);
        FD_SET(0, &server->fdSet.readfds);
        FD_SET(server->socket, &server->fdSet.readfds);
    }
    
    return 0;
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


    