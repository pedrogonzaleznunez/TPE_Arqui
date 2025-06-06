#include <syscalls.h>
#include <stdio.h>

#ifndef SHELL_H
#define SHELL_H


#define WELCOME_MESSAGE "Welcome to OreOS"
#define PROMPT_SYMBOL '$'
#define INVALID_COMMAND_MESSAGE "Invalid command."
#define MAX_COMMAND_LENGTH 50

void process_commands(char * command);

// cuando ingresa un comando no valido
void invalid_command();

// commands
void help();

void zoom_in();

void zoom_out();

void get_time();

void get_regs();





#endif