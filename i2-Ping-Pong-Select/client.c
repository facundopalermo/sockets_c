#define CHUNK_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "./headers/helper.h"
#include "./headers/client_f.h"

int main(int argc, char const *argv[]) {

    /*int port = 0;*/
    Client client;

    long rxBytes, cxBytes = 0;
    char *chunk = NULL;
    
    char *msg = getMsg(argc, argv);

    setConfig(&client, AF_INET, SOCK_STREAM, getServer(argc, argv), getPort(argc, argv));
    setConnection(&client);
    printf("Server IP: %s\n", getIP(&client));

    if(client.connection_fd == -1){
        close(client.socket);
        perror("client: connect");
        exit(1);
    }

    cxBytes = send(client.socket, msg, strlen(msg), 0);
    printf("%lu bytes enviados.\n\n", cxBytes);

    chunk = malloc(CHUNK_SIZE);
    memset(chunk, 0, CHUNK_SIZE);
    
    do {
        rxBytes = recv(client.socket, chunk, CHUNK_SIZE, 0);
        
        if(rxBytes == -1) {
            printf("recv error: ");
            perror("client: connect");
            break;
        }
        
        if(rxBytes == 0) {
            printf("\nConexion finalizada por las partes\n\n");
            break;
        }

        if(strcmp(chunk,"PONG") == 0) {
            printf("%s\n", chunk);
            break;
        }else{
            printf("Server dice: %s\n", chunk);
            break;
        }

        memset(chunk, 0, CHUNK_SIZE);
        
    } while(1);

    close(client.socket);
    freeaddrinfo(client.server);
    free(chunk);

    return 0;
}