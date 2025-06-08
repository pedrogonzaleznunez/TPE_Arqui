#ifndef SHELL_H
#define SHELL_H

#define WELCOME_MESSAGE         "Welcome to OreOS"
#define PROMPT_SYMBOL           "$"
#define INVALID_COMMAND_MESSAGE "Invalid command."
#define MAX_COMMAND_LENGTH      50

void shell(void);

void processCommands(char *command);

// cuando ingresa un comando no valido
void invalidCommand();

// commands
void help();

void zoomIn();

void zoomOut();

void getTime();

void getRegs();

void clear();

void printRegs();

#endif