#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct {
    int socket;
    struct sockaddr_in address;
} clientDataT;

int clientSockets[MAX_CLIENTS];
void* handleClient(void* arg);
int start(void);

#endif // SERVER_H