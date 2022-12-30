#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>

#include "headers/helper.h"

int getPort(int argc, char const *argv[]) {

    int i = 0;

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i],"-p") == 0) {
            return atoi(argv[i + 1]);
        }
    }

    return 8080;
}

char * getServer(int argc, char const *argv[]) {

    int i = 0;

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i],"-s") == 0) {
            return (char *)(argv[i + 1]);
        }
    }

    return "localhost";
}

char * getMsg(int argc, char const *argv[]) {

    int i = 0;

    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i],"-m") == 0) {
            return (char *)(argv[i + 1]);
        }
    }

    return "nada";
}

struct addrinfo * getServerAddress (int socket_domain, int socket_type, int port) {

    int ret = 0;
    char c_port[1024];
    struct addrinfo hints;
    struct addrinfo *serverInfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family 	= socket_domain;
    hints.ai_socktype 	= socket_type;
    hints.ai_flags 		= AI_PASSIVE;

    sprintf(c_port, "%d", port);
    getaddrinfo(NULL, c_port, &hints, &serverInfo);

    if(serverInfo == NULL) {
        
        printf("Local ip address coudn't be obtained.\n");
        exit(EXIT_FAILURE);
    }
    
    if(ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        freeaddrinfo(serverInfo);
        exit(EXIT_FAILURE);
    }
    
    return serverInfo;
}

char * getHeaders(int code, char * message, char * content_type, long content_lenght) {
    /*
    char * response = NULL;
    char * responseHeaders = "HTTP/1.1 %d %s\r\nContent-type:%s\r\nContent-lenght: %ld\r\nConnection: close\r\n\r\n";

    response = malloc(1024);
    memset(response, 0, 1024);

    sprintf(response, responseHeaders, code, message, content_type, content_lenght);

    return response;

    */
    char * response = NULL;
    char * responseHeaders = NULL;

    if(content_type == NULL){
        responseHeaders = "HTTP/1.1 %d %s\r\nContent-type:%s\r\nContent-lenght: %ld\r\nConnection: close\r\n\r\n";
    }else{
        responseHeaders = "HTTP/1.1 %d %s\r\nConnection: close\r\n\r\n";
    }

    response = malloc(1024);
    memset(response, 0, 1024);

    sprintf(response, responseHeaders, code, message, content_type, content_lenght);

    return response;
}

char * getResourse (char * string) {

    char *token = strtok(string, "/");
    token = strtok(NULL, " ");
    return token;
}
