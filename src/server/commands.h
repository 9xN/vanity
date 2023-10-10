#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int helpMenu(void);
int printBanner(void);
int clearScreen(void);
int shutdownServer(void);
int listClients(void);
int serverInfo(void);
int selectClient(const char* args);
extern int shutdownStatus;

#endif /* COMMANDS_H */