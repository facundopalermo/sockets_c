#ifndef __SERVER_F_H
#define __SERVER_F_H

#include <netinet/in.h>
#include <sys/types.h>

#define BACKLOG 10
#define CHUNK_SIZE 1024

typedef struct config
{
    int port;
    int socket;
    int connection_fd;
    char sendBuff[1025];

    struct addrinfo *addr;

}Server;

/**
 * @brief Configuracion inicial del servidor v4
 * @param server struct Server que contiene parametros port y socket
 * @param socket_domain Familia del socket: AF_UNIX, AF_LOCAL, AF_UNIX, AF_INET,AF_INET6, AF_AX25, AF_IPX, AF_APPLETALK, AF_X25, AF_DECnet
 * @param socket_type Tipo de socket: SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
 * @param port Puerto
 * @param flag Admite diferentes flags para fcntl(): O_NONBLOCK
 */
int setConfig(Server *server, int socket_domain, int socket_type, int port, int flag);

void thread(void * param);

#endif