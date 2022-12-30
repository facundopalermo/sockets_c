#ifndef __HELPER_H
#define __HELPER_H

#include <netdb.h>

    int getPort(int argc, char const *argv[]);
    char * getServer(int argc, char const *argv[]);
    char * getMsg(int argc, char const *argv[]);
    int getPool(int argc, char const *argv[]);
    struct addrinfo * getServerAddress (int socket_domain, int socket_type, int port);
    char * getHeaders(int code, char * message, char * content_type, long content_lenght);
    char * getResourse (char * string);
    
#endif