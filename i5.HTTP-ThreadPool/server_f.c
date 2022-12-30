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

    server->addr = getServerAddress(socket_domain, socket_type, port);
    server->socket = socket(server->addr->ai_family, server->addr->ai_socktype, server->addr->ai_protocol);
    server->port = port;
    memset(server->sendBuff, '0', sizeof(server->sendBuff));
    
    bind(server->socket, server->addr->ai_addr, server->addr->ai_addrlen);
    listen(server->socket, BACKLOG);
    
    return 0;
}

void thread(void * param) {

    char *chunk = NULL;
    char *response = NULL;
    char file[1024] = {'\0'};
    int imagefd = 0;
    int ret = 0;
    ssize_t readed;
    struct stat fileStats;

    int fd = *(int*)(param);
    

    printf("Conexion establecida en socket %d\n", fd);
    
    chunk = malloc(CHUNK_SIZE);
    memset(chunk, 0, CHUNK_SIZE);

    readed = recv(fd, chunk, CHUNK_SIZE, 0);
    if(readed < 1) {

        printf("Conexion finalizada en descriptor: %d\n", fd);

    }else{

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
            }else{
                printf("archivo enviado\n");
            }
        }
    }   
    close(fd);
    close(imagefd);
    free(chunk);
}