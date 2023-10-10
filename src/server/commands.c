#include "commands.h"
#include "helpers.h"
#include "server.h"

int shutdownStatus = 0;

int helpMenu(void) {
    const char* helpMenu[] = {" Commands          Description\n", " --------          -----------\n", " ?/help            Displays available commands.\n", " clients/list      List connected clients and their identifiers.\n", " select/bind <id>  Allows you to bind to a specified client.\n", " server/info       Shows details about the CnC server.\n", " banner/ascii      Displays the ASCII banner.\n", " clear/cls         Clears the screen.\n", " shutdown/quit     Shuts down the server.\n"};
    char tempBuffer[BUFFER_SIZE];
    for (int i = 0; i < sizeof(helpMenu) / sizeof(helpMenu[0]); i++) {
        sprintf(tempBuffer, "%s", helpMenu[i]);
        sprint(tempBuffer);
    }
    fflush(stdout);
    return 0;
};

int printBanner(void) {
    const char* banner[] = {"       \033[38;5;160m┬\033[38;5;160m \033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m┌\033[38;5;160m─\033[38;5;160m┐\033[38;5;160m \033[38;5;160m┌\033[38;5;160m┐\033[38;5;160m┌\033[38;5;160m \033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m \033[38;5;160m┌\033[38;5;160m┬\033[38;5;160m┐\033[38;5;160m \033[38;5;160m┬\033[38;5;160m \033[38;5;160m┬\n", "       \033[38;5;160m└\033[38;5;160m┐\033[38;5;160m┌\033[38;5;160m┘\033[38;5;161m \033[38;5;161m├\033[38;5;161m─\033[38;5;161m┤\033[38;5;161m \033[38;5;161m│\033[38;5;161m│\033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m \033[38;5;161m│\033[38;5;161m \033[38;5;161m \033[38;5;161m└\033[38;5;161m┬\033[38;5;161m┘\n", "       \033[38;5;161m \033[38;5;161m└\033[38;5;161m┘\033[38;5;161m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m┘\033[38;5;162m└\033[38;5;162m┘\033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m \033[38;5;162m \033[38;5;162m \033[38;5;162m┴\033[38;5;162m", colours[6], " v", VERSION, "\n", "\033[38;5;162m  ~~ Made By: \033[38;5;160mhttps://github.com/9xN\033[38;5;162m ~~\n"};
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
    sprintf(tempBuffer, " Shutting down server...");
    sprint(tempBuffer);
    shutdownStatus = 1;
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

int serverInfo(void) {
    char tempBuffer[BUFFER_SIZE];
    int connectedClients = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientSockets[i] > 0) {
            connectedClients++;
        }
    }

    sprintf(tempBuffer, " Server information\n");
    sprint(tempBuffer);
    sprintf(tempBuffer, " ------------------\n");
    sprint(tempBuffer);
    sprintf(tempBuffer, " Server Version: %s\n", VERSION);
    sprint(tempBuffer);
    sprintf(tempBuffer, " Max Buffer Size: %d\n", BUFFER_SIZE);
    sprint(tempBuffer);
    sprintf(tempBuffer, " Max Clients: %d\n", MAX_CLIENTS);
    sprint(tempBuffer);
    sprintf(tempBuffer, " Server Address: %d\n", PORT);
    sprint(tempBuffer);
    sprintf(tempBuffer, " Number of Clients: %d\n", connectedClients);
    sprint(tempBuffer);
    sprintf(tempBuffer, " Number of Commands: %lu\n", sizeof(&helpMenu) - 1);
    sprint(tempBuffer);
    return 0;
};
int selectClient(const char* args) {
    char buffer[BUFFER_SIZE], tempBuffer[BUFFER_SIZE], clientAddressPort[BUFFER_SIZE], clientString[BUFFER_SIZE];
    int clientID = atoi(args);
    if (clientID >= 1 && clientID <= MAX_CLIENTS && clientSockets[clientID - 1] > 0) {
        int selectedClientSocket = clientSockets[clientID - 1];
        struct sockaddr_in clientSock;
        socklen_t clientSockLen = sizeof(clientSock);
        sprintf(clientString, "%s%d%s", colours[6], clientID, colours[3]);
        sprintf(clientAddressPort, "%s%s%s:%s%d%s", colours[6], inet_ntoa(clientSock.sin_addr), colours[3], colours[6], ntohs(clientSock.sin_port), colours[3]);
        if (getpeername(selectedClientSocket, (struct sockaddr*)&clientSock, &clientSockLen) == 0) {
            sprintf(tempBuffer, " Binding to client %s at %s\n", clientString, clientAddressPort);
            sprint(tempBuffer);
            sprintf(tempBuffer, "%s/%s%s", colours[4], colours[3], clientAddressPort);
            printPrompt(tempBuffer);
        } else {
            sprintf(tempBuffer, " Failed to bind to client %s at %s\n", clientString, clientAddressPort);
            sprint(tempBuffer);
            printPrompt("");
        }
        while (1) {
            sprintf(tempBuffer, "%s/%s%s", colours[4], colours[3], clientAddressPort);
            getInput(buffer, BUFFER_SIZE);
            printPrompt(tempBuffer);

            if (strcmp(buffer, "exit") == 0) {
                printf("\n");
                sprintf(tempBuffer, " Unbinding from client %s at %s\n", clientString, clientAddressPort);
                sprint(tempBuffer);
                break;
            } else {
                if (send(selectedClientSocket, buffer, strlen(buffer), 0) < 0) {
                    sprintf(tempBuffer, " Failed to send data to client %s at %s\n", clientString, clientAddressPort);
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
