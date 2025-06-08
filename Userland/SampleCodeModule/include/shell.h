#ifndef SHELL_H
#define SHELL_H

#define WELCOME_MESSAGE         "Welcome to OreOS"
#define PROMPT_SYMBOL           "$"
#define INVALID_COMMAND_MESSAGE "Invalid command."
#define MAX_COMMAND_LENGTH      50
#define TIME_ARGS_MSG "Invalid arguments...\nValid arguments:\nnone\nh for hour\nd for date\ny for year"

void shell(void);

void processCommands(char *command);

// cuando ingresa un comando no valido
void invalidCommand();

int checkArguments(int argsExpected, int argsRead, char *command);

// commands
void help();

void zoomIn();

void zoomOut();

void getTime();

void getRegs();

void clear();

void printRegs();

#endif