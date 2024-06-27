# Makefile

CC = gcc
CFLAGS = -Wall -std=c99 -pthread

# Target to compile both server and client
all: server client

# Target for compiling the server
server: server.c
	$(CC) $(CFLAGS) -o server server.c

# Target for compiling the client
client: client.c
	$(CC) $(CFLAGS) -o client client.c

run_server: server 
	./server 8000

# Target to clean the build
clean:
	rm -f server client

.PHONY: all clean