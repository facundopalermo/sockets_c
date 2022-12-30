# Crea los programas
# ejecutables:
#	server
#	client

CC = gcc
FG = -Wall -O1 -std=gnu89 -pedantic-errors -Werror
FGT = $(FG) -pthread

HEADERS_ROUTE = ../headers/
FG_HEADERS = -I$(HEADERS_ROUTE)

#	compila los programas y limpia los archivo objeto
.PHONY: all
all: server client clean


#	MAIN DE PROGRAMAS
server: ../server.c server_f.o threads.o helper.o
	$(CC) $(FGT) -o server ../server.c helper.o threads.o server_f.o

client: ../client.c client_f.o helper.o
	$(CC) $(FG) -o client ../client.c client_f.o helper.o $(FG_HEADERS)


server_f.o: ../server_f.c $(HEADERS_ROUTE)server_f.h
	$(CC) $(FG) -c ../server_f.c $(FG_HEADERS)

client_f.o: ../client_f.c $(HEADERS_ROUTE)client_f.h
	$(CC) $(FG) -c ../client_f.c $(FG_HEADERS)

#	COMUNES *.o
threads.o: ../threads.c $(HEADERS_ROUTE)threads.h
	$(CC) $(FG) -c ../threads.c $(FG_HEADERS)

helper.o: ../helper.c $(HEADERS_ROUTE)helper.h
	$(CC) $(FG) -c ../helper.c $(FG_HEADERS)

.PHONY: clean
clean:
	rm -rf *.o