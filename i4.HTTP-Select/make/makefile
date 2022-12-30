# Crea los programas
# ejecutables:
#	server

CC = gcc
FG = -Wall -O1 -std=gnu89 -pedantic-errors -Werror

HEADERS_ROUTE = ../headers/
FG_HEADERS = -I$(HEADERS_ROUTE)

#	compila los programas y limpia los archivo objeto
.PHONY: all
all: server clean


#	MAIN DE PROGRAMAS
server: ../server.c server_f.o helper.o
	$(CC) $(FG) -o server ../server.c helper.o server_f.o


server_f.o: ../server_f.c $(HEADERS_ROUTE)server_f.h
	$(CC) $(FG) -c ../server_f.c $(FG_HEADERS)

#	COMUNES *.o

helper.o: ../helper.c $(HEADERS_ROUTE)helper.h
	$(CC) $(FG) -c ../helper.c $(FG_HEADERS)

.PHONY: clean
clean:
	rm -rf *.o