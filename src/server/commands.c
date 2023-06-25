#include "commands.h"
#include "helpers.h"
#include "server.h"

int helpMenu(void) {
    const char* helpMenu[] = {" Commands          Description\n", " --------          -----------\n", " ?/help            Displays available commands.\n", " clients/list      List connected clients and their identifiers.\n", " select <id>       Allows you to bind to a specified client.\n", " server            Shows details about the CnC server.\n", " banner/ascii      Displays the ASCII banner.\n", " clear/cls         Clears the screen.\n", " shutdown/quit     Shuts down the server.\n"};
    char tempBuffer[BUFFER_SIZE];
    for (int i = 0; i < sizeof(helpMenu) / sizeof(helpMenu[0]); i++) {
        sprintf(tempBuffer, "%s", helpMenu[i]);
        sprint(tempBuffer);
    }
    fflush(stdout);
    return 0;
};

int printBanner(void) {
    const char* banner[] = {"       \033[38;5;160m┬\033[38;5;160m \033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m┌\033[38;5;160m─\033[38;5;160m┐\033[38;5;160m \033[38;5;160m┌\033[38;5;160m┐\033[38;5;160m┌\033[38;5;160m \033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m \033[38;5;160m┌\033[38;5;160m┬\033[38;5;160m┐\033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m┬\n", "       \033[38;5;160m└\033[38;5;160m┐\033[38;5;160m┌\033[38;5;160m┘\033[38;5;161m \033[38;5;161m├\033[38;5;161m─\033[38;5;161m┤\033[38;5;161m \033[38;5;161m│\033[38;5;161m│\033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m \033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m└\033[38;5;161m┬\033[38;5;161m┘\n", "       \033[38;5;161m \033[38;5;161m└\033[38;5;161m┘\033[38;5;161m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m┘\033[38;5;162m└\033[38;5;162m┘\033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \n", "  ~~ Made By: \033[38;5;160mhttps://github.com/9xN\033[38;5;162m ~~\n"};
    for (int i = 0; i < sizeof(banner) / sizeof(banner[0]); i++) {
        printf("%s", banner[i]);
    }
    return 0;
};

int clearScreen(void) {
    printf("\033[H\033[2J\n");
    return 0;
};

int shutdownServer(void) {
    char tempBuffer[BUFFER_SIZE];
    sprintf(tempBuffer, " Shutting down server...\n\n");
    sprint(tempBuffer);
    return 0;
};

int listClients(void) {
    char tempBuffer[BUFFER_SIZE];
    sprintf(tempBuffer, " Connected clients...");
    sprint(tempBuffer);
    printf("\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        int sd = clientSockets[i];
        if (sd > 0) {
            struct sockaddr_in clientSock;
            socklen_t clientSockLen = sizeof(clientSock);
            if (getpeername(sd, (struct sockaddr*)&clientSock, &clientSockLen) == 0) {
                sprintf(tempBuffer, " %d. %s:%d\n", i + 1, inet_ntoa(clientSock.sin_addr), ntohs(clientSock.sin_port));
                sprint(tempBuffer);
                continue;
            }
        }
    }
    return 0;
};

int selectClient(const char* args) {
    char buffer[BUFFER_SIZE], tempBuffer[BUFFER_SIZE], clientAddressPort[BUFFER_SIZE];
    int clientID = atoi(args);

    if (clientID >= 1 && clientID <= MAX_CLIENTS && clientSockets[clientID - 1] > 0) {
        int selectedClientSocket = clientSockets[clientID - 1];
        struct sockaddr_in clientSock;
        socklen_t clientSockLen = sizeof(clientSock);

        if (getpeername(selectedClientSocket, (struct sockaddr*)&clientSock, &clientSockLen) == 0) {
            sprintf(clientAddressPort, "%s:%d", inet_ntoa(clientSock.sin_addr), ntohs(clientSock.sin_port));
            sprintf(tempBuffer, " Binding to client %d at %s\n", clientID, clientAddressPort);
            sprint(tempBuffer);
            sprintf(tempBuffer, "%s/%s%s", colours[4], colours[3], clientAddressPort);
            printPrompt(tempBuffer);
        } else {
            sprintf(tempBuffer, " Failed to bind to client %d at %s\n", clientID, clientAddressPort);
            sprint(tempBuffer);
            printPrompt("");
        }
        while (1) {
            sprintf(tempBuffer, "%s/%s%s", colours[4], colours[3], clientAddressPort);
            getInput(buffer, BUFFER_SIZE);
            printPrompt(tempBuffer);

            if (strcmp(buffer, "exit") == 0) {
                printf("\n");
                sprintf(tempBuffer, " Unbinding from client %d at %s\n", clientID, clientAddressPort);
                sprint(tempBuffer);
                break;
            } else {
                if (send(selectedClientSocket, buffer, strlen(buffer), 0) < 0) {
                    sprintf(tempBuffer, " Failed to send data to client %d at %s\n", clientID, clientAddressPort);
                    sprint(tempBuffer);
                    break;
                }
            }
        }
    } else {
        sprintf(tempBuffer, " Invalid client index specified.\n");
        sprint(tempBuffer);
    }
    return 0;
}
