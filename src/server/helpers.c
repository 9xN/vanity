#include "commands.h"
#include "helpers.h"

const char* colours[] = {
    "\033[1;34m",      // lightBlue     0
    "\033[1;32m",      // lightGreen    1
    "\033[1;33m",      // lightYellow   2
    "\033[1;35m",      // brightMagenta 3
    "\033[1;36m",      // brightCyan    4
    "\033[1;35m",      // brightViolet  5
    "\033[38;5;160m",  // red1          6
    "\033[38;5;161m",  // red2          7
    "\033[38;5;162m",  // red3          8
    "\033[0m"          // reset color   9
};

void sprint(char* string) {
    printf("   %s├───%s%s", colours[0], colours[3], string);
    fflush(stdout);
}

void newPrompt(void) {
    printf("   %s┌────────────%s[%s$%s]\n", colours[0], colours[1], colours[2], colours[1]);
    fflush(stdout);
}

void printPrompt(char* clientAddressPort) {
    char tempBuffer[BUFFER_SIZE];
    sprintf(tempBuffer, "%s[%sroot%s@%svanity%s%s ~ %s", colours[0], colours[5], colours[4], colours[3], clientAddressPort, colours[5], colours[0]);
    sprint(tempBuffer);
    fflush(stdout);
}

char* fmtClientPort(char* clientAddressPort, struct sockaddr_in clientAddress) {
    sprintf(clientAddressPort, "%s%s%s:%s%d%s", colours[6], inet_ntoa(clientAddress.sin_addr), colours[3], colours[6], ntohs(clientAddress.sin_port), colours[3]);
    return clientAddressPort;
}

char* fmtClientID(char* clientID, int id) {
    sprintf(clientID, "%s%d%s", colours[6], id, colours[3]);
    return clientID;
}

char* getInput(char* inputBuffer, int bufferSize) {
    fgets(inputBuffer, bufferSize, stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
    return inputBuffer;
}

int handleCommand(const char* command, const char* args) {
    if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0) {
        helpMenu();
        return 0;
    } else if (strcmp(command, "banner") == 0 || strcmp(command, "ascii") == 0) {
        clearScreen();
        printBanner();
        newPrompt();
        return 0;
    } else if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0) {
        clearScreen();
        newPrompt();
        return 0;
    } else if (strcmp(command, "shutdown") == 0 || strcmp(command, "quit") == 0) {
        shutdownServer();
        return 2;
    } else if (strcmp(command, "list") == 0 || strcmp(command, "clients") == 0) {
        listClients();
        return 0;
    } else if (strcmp(command, "server") == 0 || strcmp(command, "info") == 0) {
        serverInfo();
        return 0;
    } 
    else if (strcmp(command, "select") == 0 || strcmp(command, "bind") == 0) {
        selectClient(args);
        return 0;
    }
    else if (strcmp(command, "boot") == 0 || strcmp(command, "kick") == 0) {
        removeClient(args);
        return 0;
    }
    else {
        return 1;
    }
    printPrompt("");
}
