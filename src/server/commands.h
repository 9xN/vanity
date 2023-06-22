#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

struct cmdStruct {
    const char* command;
    int (*functionPtr)();
};

struct cmdStruct cmdStruct[];

int helpMenu();
int printBanner();
int clearScreen();
int shutdownServer();
int listClients();
int selectClient(const char* args);

#endif /* COMMANDS_H */