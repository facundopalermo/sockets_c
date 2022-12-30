#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <fcntl.h>

#include "headers/helper.h"
#include "headers/server_f.h"

int main(int argc, char const *argv[]) {

    int fdmax = 0;
    
    Server server;

    setConfig(&server, AF_INET, SOCK_STREAM, getPort(argc, argv), O_NONBLOCK);
    printf("Escuchando en puerto %d...\n", server.port);

    fdmax = server.socket;

    while(1)
    {
        server.fdSet.tempreadfds = server.fdSet.readfds;

        if(serverSelect(&server, &fdmax) == -1){
            return -1;
        }
    }

    return 0;
}

