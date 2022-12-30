#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

#include <sys/stat.h>
#include <sys/sendfile.h>

#include "headers/server_f.h"
#include "headers/helper.h"

int setConfig(Server *server, int socket_domain, int socket_type, int port, int flag){

    int yes = 1;

    server->addr = getServerAddress(socket_domain, socket_type, port);
    server->socket = socket(server->addr->ai_family, server->addr->ai_socktype, server->addr->ai_protocol);
    server->port = port;
    memset(server->sendBuff, '0', sizeof(server->sendBuff));
    
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
    char *response = NULL;
    ssize_t readed;

    struct stat fileStats;
   
    struct sockaddr sockaddClient;
    socklen_t sockaddClientLength;
    int socketNewCx;

    char file[1024] = {'\0'};
    /*int imagefd = open("./image.jpg", O_RDONLY);*/
    int imagefd = 0;
    

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
                    sprintf(file, "./images/%s", getResourse(chunk));
                    printf("%s\n", file);

                    if((imagefd = open(file, O_RDONLY)) == -1){

                        response = getHeaders(404, "NOT FOUND", NULL, 0);
                        printf("%s\n", response);
                        send(fd, response, strlen(response), 0);
                        
                    }else{

                        fstat(imagefd, &fileStats);
                        /* response */
                        response = getHeaders(200, "OK", "image/jpeg", fileStats.st_size);
                        printf("%s\n", response);
                        send(fd, response, strlen(response), 0);

                        ret = sendfile(fd, imagefd, NULL, fileStats.st_size);

                        if(ret == -1) {
                            perror("sendfile");
                            return -1;
                        }else{
                            printf("archivo enviado");
                        }
                    }

                    close(fd);
                    FD_CLR(fd, &server->fdSet.readfds);
                    close(imagefd);
                    
                }
            }
        }
    }
    printf("\n");
    free(chunk);
    free(response);
    return 0;
}



    