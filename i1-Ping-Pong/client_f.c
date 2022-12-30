#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#include "headers/client_f.h"

int setConfig(Client *client, int socket_domain, int socket_type, char *addr, int port) {

    int ret;
    char c_port[1024];
    struct addrinfo hints;
    struct addrinfo *serverInfo = NULL;
    struct addrinfo *currentAddr = NULL;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family 	= socket_domain;
	hints.ai_socktype 	= socket_type;

    client->server = NULL;
    sprintf(c_port, "%d", port);

    ret = getaddrinfo(addr, c_port, &hints, &serverInfo);
    if(ret != 0) {
    
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        freeaddrinfo(serverInfo);

        return -1;
    }

    for (currentAddr = serverInfo; currentAddr != NULL; currentAddr = currentAddr->ai_next) {
        
        if(client->server == NULL && currentAddr->ai_family == socket_domain) {
            client->server = currentAddr;
            break;
        }
    }

    client->socket = socket(client->server->ai_family, client->server->ai_socktype, client->server->ai_protocol);
    return 1;

}

void setConnection(Client *client) {
    client->connection_fd = connect(client->socket, client->server->ai_addr, client->server->ai_addrlen);
}

const char * getIP (Client *client) {

    char ipAddress[100];
    void *rawAddress = NULL;

    if(client->server->ai_family == AF_INET) {
        rawAddress = &((struct sockaddr_in *) client->server->ai_addr)->sin_addr;
    } else {
        rawAddress = &((struct sockaddr_in6 *) client->server->ai_addr)->sin6_addr;
    }
    
    return inet_ntop(client->server->ai_family, rawAddress, ipAddress, sizeof(ipAddress));  
}