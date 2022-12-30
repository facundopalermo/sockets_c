#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>

#include "headers/helper.h"
#include "headers/threads.h"
#include "headers/server_f.h"

int main(int argc, char const *argv[]) {

    int i = 0;
    Server server;
    pthread_t *hilo;

    hilo = (pthread_t*) malloc (sizeof(pthread_t) * 1000);

    setConfig(&server, AF_INET, SOCK_STREAM, getPort(argc, argv));
    printf("Escuchando en puerto %d...\n", server.port);
    
    while(1)
    {
        server.connection_fd = accept(server.socket, (struct sockaddr*)NULL, NULL);

        if(server.connection_fd < 0) {
            close(server.socket);
            perror("client: connect");
            exit(1);
        }
        
        if(crearThread(&hilo[i], thread, &server.connection_fd) == -1){
            return(-1);
        }

        i++;
    }

    free(hilo);
    return 0;
}

