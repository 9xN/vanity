CC = gcc
CFLAGS = -pthread

all: server client

server: vanity.c
	$(CC) -o vanity vanity.c $(CFLAGS)

client: src/client/client.c
	$(CC) -o client src/client/client.c

clean:
	rm -f vanity client
