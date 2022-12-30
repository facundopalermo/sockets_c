#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>


#include "headers/server_f.h"

void setConfig(Server *server, int socket_domain, int socket_type, int port){

    server->socket = socket(socket_domain, socket_type, 0);
    server->port = port;

    memset(&server->addr, '0', sizeof(server->addr));
    memset(server->sendBuff, '0', sizeof(server->sendBuff));

    server->addr.sin_family = socket_domain;
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr.sin_port = htons(server->port);
    
    bind(server->socket, (struct sockaddr*)&server->addr, sizeof(server->addr));

    listen(server->socket, BACKLOG);

}

void * thread(void * param) {

    char *chunk = NULL;
    char *pong = "PONG";
    char *noentendo = "NO ENTENDO";
    long rxBytes = 0;
    int connectionFd = *(int*)(param);

    printf("Conexion establecida en socket %d\n", connectionFd);
    
    chunk = malloc(CHUNK_SIZE);
    memset(chunk, 0, CHUNK_SIZE);
    
    do {
    
        rxBytes = recv(connectionFd, chunk, CHUNK_SIZE, 0);
        
        if(rxBytes == -1) {
            printf("recv error: ");
            perror("client: connect");
            break;
        }
        
        if(rxBytes == 0) {
            printf("\nConexion finalizada por las partes \n\n");
            break;
        }

        if(strcmp(chunk,"PING") == 0) {
            send(connectionFd, pong, strlen(pong), 0);
        } else {
            send(connectionFd, noentendo, strlen(noentendo), 0);
        }
    
        printf("%s", chunk);
        memset(chunk, 0, CHUNK_SIZE);
        
    } while(1);
    
    free(chunk);
    close(connectionFd);
    pthread_exit(0);
}


    