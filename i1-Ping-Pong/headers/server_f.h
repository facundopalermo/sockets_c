#ifndef __SERVER_F_H
#define __SERVER_F_H

#include <netinet/in.h>


#define BACKLOG 10
#define CHUNK_SIZE 1024

typedef struct config
{
    int port;
    int socket;
    int connection_fd;
    char sendBuff[1025];

    struct sockaddr_in addr;

}Server;

/**
 * @brief Configuracion inicial del servidor v1
 * @param server struct Server que contiene parametros port y socket
 * @param socket_domain Familia del socket: AF_UNIX, AF_LOCAL, AF_UNIX, AF_INET,AF_INET6, AF_AX25, AF_IPX, AF_APPLETALK, AF_X25, AF_DECnet
 * @param socket_type Tipo de socket: SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
 * @param port Puerto
 */
void setConfig(Server *server, int socket_domain, int socket_type, int port);

void * thread(void * param); 

#endif