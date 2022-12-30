#ifndef __CLIENT_F_H
#define __CLIENT_F_H

#include <netinet/in.h>
#include <netdb.h>

typedef struct config
{
    int socket;
    int connection_fd;

    struct addrinfo *server;

}Client;

/**
 * @brief Configuracion inicial del cliente v1
 * 
 * @param client struct Client que contiene parametros socket y server
 * @param socket_domain Familia del socket: AF_UNIX, AF_LOCAL, AF_UNIX, AF_INET,AF_INET6, AF_AX25, AF_IPX, AF_APPLETALK, AF_X25, AF_DECnet
 * @param socket_type Tipo de socket: SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
 * @param addr Direccion del servidor, localhost es valido
 * @param port Puerto
 * @return int Retorna -1 si getaddrinfo da error
 */
int setConfig(Client *client, int socket_domain, int socket_type, char *addr, int port);

/**
 * @brief Conecta el cliente al servidor y asigna la conexion a connection_fd 
 * 
 * @param client struct Client que contiene parametros socket y server
 */
void setConnection(Client *client);

const char * getIP (Client *client);

#endif