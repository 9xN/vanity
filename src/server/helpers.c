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

void newPrompt() {
    printf("   %s┌────────────%s[%s$%s]\n", colours[0], colours[1], colours[2], colours[1]);
    fflush(stdout);
}

void printPrompt(char* clientAddressPort) {
    char tempBuffer[BUFFER_SIZE];
    sprintf(tempBuffer, "root%s@%svanity%s%s ~ %s", colours[4], colours[3], clientAddressPort, colours[5], colours[0]);
    sprint(tempBuffer);
    fflush(stdout);
}

char* getInput(char* inputBuffer, int bufferSize) {
    fgets(inputBuffer, bufferSize, stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
    return inputBuffer;
}

void execCmd(const char* command) {
    int numCommands = sizeof(cmdStruct) / sizeof(cmdStruct[0]);
    for (int i = 0; i < numCommands; i++) {
        if (strcmp(command, cmdStruct[i].command) == 0) {
            cmdStruct[i].functionPtr();
            return;
        }
    }
}

int handleCommand(const char* command, const char* args) {
    char buffer[BUFFER_SIZE];
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
    } else if (strcmp(command, "shutdown") == 0 || strcmp(command, "exit") == 0) {
        shutdownServer();
        return 2;
    } else if (strcmp(command, "list") == 0 || strcmp(command, "clients") == 0) {
        listClients();
        return 0;
    } else if (strcmp(command, "select") == 0) {
        selectClient(args);
        return 0;
    } else {
        return 1;
    }
    printPrompt("");
}
