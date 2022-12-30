#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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