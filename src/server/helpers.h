#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <netinet/in.h>

const char* colours[];

void sprint(char* string);
void newPrompt(void);
void printPrompt(char* clientAddressPort);
char* getInput(char* inputBuffer, int bufferSize);
//void execCmd(const char* command);
int handleCommand(const char* command, const char* args);

#endif // HELPERS_H
