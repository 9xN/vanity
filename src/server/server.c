#include "config.h"
#include "server.h"
#include "commands.c"
#include "helpers.c"

void* handleClient(void* arg) {
    clientDataT* clientData = (clientDataT*)arg;
    int clientSocket = clientData->socket, readSize;
    struct sockaddr_in clientAddress = clientData->address;
    char buffer[BUFFER_SIZE], tempBuffer[BUFFER_SIZE];

    printf("\n");
    sprintf(tempBuffer, " Client connected: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
    sprint(tempBuffer);
    printPrompt("");

    while (1) {
        readSize = read(clientSocket, buffer, BUFFER_SIZE);
        if (readSize <= 0) {
            printf("\n");
            sprintf(tempBuffer, " Client disconnected: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
            sprint(tempBuffer);
            printPrompt("");
            break;
        }
        printf("\n");
        sprintf(tempBuffer, " Received from client %s:%d: %s\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), buffer);
        sprint(tempBuffer);
        //printPrompt("");
    }

    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clientSockets[i] == clientSocket) {
            clientSockets[i] = 0;
            break;
        }
    }

    close(clientSocket);
    free(clientData);
    pthread_exit(NULL);
}

int start(void) {
    int serverFd, activity, readSize, maxSd, sd, i;
    struct sockaddr_in serverAddr;
    socklen_t addrlen;
    char buffer[BUFFER_SIZE];
    fd_set readFds;

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    if (listen(serverFd, 3) < 0) {
        perror("listen failed");
        exit(1);
    }

    for (i = 0; i < MAX_CLIENTS; i++) {
        clientSockets[i] = 0;
    }
    maxSd = serverFd;

    clearScreen();
    printBanner();
    printf("     ~~ Listening on: %s%s:%d%s ~~\n\n", colours[6], inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), colours[8]);
    newPrompt();
    printPrompt("");
    while (1) {
        FD_ZERO(&readFds);
        FD_SET(serverFd, &readFds);
        FD_SET(STDIN_FILENO, &readFds);

        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = clientSockets[i];
            if (sd > 0) {
                FD_SET(sd, &readFds);
            }
            if (sd > maxSd) {
                maxSd = sd;
            }
        }

        activity = select(maxSd + 1, &readFds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }

        if (FD_ISSET(serverFd, &readFds)) {
            int newSocket;
            addrlen = sizeof(serverAddr);

            if ((newSocket = accept(serverFd, (struct sockaddr*)&serverAddr, &addrlen)) < 0) {
                perror("accept failed");
                exit(1);
            }

            for (i = 0; i < MAX_CLIENTS; i++) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = newSocket;

                    clientDataT* clientData = (clientDataT*)malloc(sizeof(clientDataT));
                    clientData->socket = newSocket;
                    clientData->address = serverAddr;

                    pthread_t clientThread;

                    if (pthread_create(&clientThread, NULL, handleClient, (void*)clientData) < 0) {
                        perror("Failed to create client thread");
                        exit(1);
                    }

                    if (pthread_detach(clientThread) < 0) {
                        perror("Failed to detach client thread");
                        exit(1);
                    }

                    break;
                }
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readFds)) {
            getInput(buffer, BUFFER_SIZE);
            char command[BUFFER_SIZE], args[BUFFER_SIZE];
            sscanf(buffer, "%s %[^\n]", command, args);

            int stat = handleCommand(command, args);
            if (stat == 1) {
                for (i = 0; i < MAX_CLIENTS; i++) {
                    sd = clientSockets[i];
                    if (sd > 0) {
                        //printf("Sending to client %d: %s\n", i, buffer);
                        if (send(sd, buffer, strlen(buffer), 0) == -1) {
                            perror("send");
                        }
                    }
                }
            } else if (stat == 2) {
                for (i = 0; i < MAX_CLIENTS; i++) {
                    sd = clientSockets[i];
                    if (sd > 0) {
                        close(sd);
                    }
                }
                close(serverFd);
                exit(0);
            }
        }
        printPrompt("");
        fflush(stdout);
    }
    return 0;
}