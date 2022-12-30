#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <fcntl.h>

#include "headers/helper.h"
#include "headers/server_f.h"

#include "headers/threadpool.h"

int main(int argc, char const *argv[]) {
   
    Server server;
    threadpool_t *pool = threadpool_create(getPool(argc, argv), 5, 0);

    if(setConfig(&server, AF_INET, SOCK_STREAM, getPort(argc, argv), 0) == -1){
        printf("error");
    }
    printf("Escuchando en puerto %d...\n", server.port);

    while(1)
    {
        server.connection_fd = accept(server.socket, (struct sockaddr*)NULL, NULL);
        if(server.connection_fd < 0) {
            close(server.socket);
            perror("client: connect");
            exit(1);
        }

        threadpool_add(pool, &thread, &server.connection_fd, 0);
    }
    
    threadpool_destroy(pool, 0);
    return 0;
}

