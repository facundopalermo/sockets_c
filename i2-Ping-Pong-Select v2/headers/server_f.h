#ifndef __SERVER_F_H
#define __SERVER_F_H

#include <netinet/in.h>
#include <sys/types.h>

#define BACKLOG 10
#define CHUNK_SIZE 1024

typedef struct fd
{
    fd_set readfds;
    fd_set tempreadfds;
    fd_set writefds;
    fd_set exceptionfds;

}fdSet;

typedef struct config
{
    int port;
    int socket;
    int connection_fd;

    struct addrinfo *addr;
    fdSet fdSet;

}Server;

/**
 * @brief Configuracion inicial del servidor v3
 * @param server struct Server que contiene parametros port y socket
 * @param socket_domain Familia del socket: AF_UNIX, AF_LOCAL, AF_UNIX, AF_INET,AF_INET6, AF_AX25, AF_IPX, AF_APPLETALK, AF_X25, AF_DECnet
 * @param socket_type Tipo de socket: SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
 * @param port Puerto
 * @param flag Admite diferentes flags para fcntl(): O_NONBLOCK
 */
int setConfig(Server *server, int socket_domain, int socket_type, int port, int flag);

int serverSelect(Server *server, int *fdmax);

#endif