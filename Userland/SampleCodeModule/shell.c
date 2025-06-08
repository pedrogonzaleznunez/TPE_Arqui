#include <myStrings.h>
#include <pongis.h>
#include <shell.h>
#include <stdio.h>
#include <syscalls.h>

#include <stddef.h>

#define MAX_LINES_SAVED 40

void activateShell(void);
int isEmpty(char *command);
void putLineInBuffer(char *line, int isCommand);
void printReg(char *regName, int64_t value);
char *getCommandFromHistory(int historyIndex);
void checkArguments(int argsExpected, int argsRead, char *command, char *instruction,
                    char *arg1);

extern void throwZeroDivisionException(void);
extern void throwInvalidOpcodeException(void);

typedef struct line {
    char characters[MAX_COMMAND_LENGTH];
    int isCommand;
} line_t;

static line_t lineBuffer[MAX_LINES_SAVED];
static int firstLine = 0;
static int lastLine = 0;
static int currentHistoryIndex = -1;// navegación de historial
static int totalLines = 0;
static int commandCount = 0;

void shell(void) {
    puts(WELCOME_MESSAGE);
    putLineInBuffer(WELCOME_MESSAGE, 0);
    help();

    firstLine = 0;
    lastLine = 0;
    totalLines = 1;
    currentHistoryIndex = -1;

    while (1) {// agregar forma de salir?? con un boolean (int) done/exit/quit
        activateShell();
    }
}

void activateShell(void) {
    printf("%s ", PROMPT_SYMBOL);

    char shell_buffer[MAX_COMMAND_LENGTH] = {0};
    int i = 0;
    char c;
    int read = 1;

    currentHistoryIndex = -1;

    while (read && i < MAX_COMMAND_LENGTH) {
        sys_read(0, &c, 1);// leer un carácter

        if (c == '\n') {// dejar de leer y procesar
            shell_buffer[i] = '\0';
            read = 0;
            putchar(c);
        } else if (c == '\b') {// backspace
            if (i > 0) {
                i--;
                shell_buffer[i] = '\0';
                // syscall que llame a deleteChar en videoDriver
                sys_delete_char();
            }
        } else if (c == ARROW_DOWN_CODE || c == ARROW_UP_CODE) {
            // si no hay comandos, no analiza
            if (commandCount != 0) {
                if (c == ARROW_UP_CODE) {
                    if (currentHistoryIndex == -1) {
                        currentHistoryIndex = 0;// empieza en el comando más nuevo
                    } else if (currentHistoryIndex < commandCount - 1) {
                        currentHistoryIndex++;// avanza en los comandos
                    }
                } else {// ARROW_DOWN_CODE
                    if (currentHistoryIndex > 0) {
                        currentHistoryIndex--;
                    } else if (currentHistoryIndex == 0) {
                        currentHistoryIndex = -1;// vuelve a línea vacía
                    }
                }

                while (i > 0) {// vacía la línea en la que está para luego sobreescribirla
                    shell_buffer[i--] = '\0';
                    sys_delete_char();
                }

                // si no estamos en línea vacía, mostrar el comando del historial
                if (currentHistoryIndex >= 0) {
                    char *command = getCommandFromHistory(currentHistoryIndex);
                    if (command != NULL) {
                        strcpy(shell_buffer, command);
                        i = strlen(shell_buffer);
                        printf("%s", shell_buffer);
                    }
                } else {
                    // Línea vacía
                    shell_buffer[0] = '\0';
                    i = 0;
                }
            }

        } else {
            //if(i < MAX_COMMAND_LENGTH - 1){
            shell_buffer[i++] = c;
            putchar(c);// mostrar en pantalla
            // }
        }
        // ignorar otros caracteres
    }

    shell_buffer[i] = '\0';
    if (i == MAX_COMMAND_LENGTH) {
        shell_buffer[i - 1] = 0;
        putchar('\n');
    }

    if (!isEmpty(shell_buffer)) {
        char fullLine[MAX_COMMAND_LENGTH + strlen(PROMPT_SYMBOL)];
        strcpy(fullLine, PROMPT_SYMBOL);
        strcat(fullLine, shell_buffer);
        putLineInBuffer(fullLine, 1);
    }

    processCommands(shell_buffer);
}

char *getCommandFromHistory(int historyIndex) {
    int commandsFound = 0;
    int index = (lastLine + MAX_LINES_SAVED - 1) % MAX_LINES_SAVED;
    int searched = 0;

    // busco el comando en la posición historyIndex (0 = más reciente)
    while (searched < totalLines) {
        if (lineBuffer[index].isCommand == 1) {
            if (commandsFound == historyIndex) {
                // devuelvo la parte del comando sin el prompt
                return lineBuffer[index].characters + strlen(PROMPT_SYMBOL);
            }
            commandsFound++;
        }
        index = (index + MAX_LINES_SAVED - 1) % MAX_LINES_SAVED;
        searched++;
    }

    return NULL;
}

void processCommands(char *command) {
    char instruction[MAX_COMMAND_LENGTH];// ver tema longitud
    char arg1[MAX_COMMAND_LENGTH];


    printf("[DEBUG-BEFORE TRIM] command='%s'\n", command);
    trim(command);
    printf("[DEBUG-AFTER TRIM] command='%s'\n", command);

    // sscanf no funciona como esperado

    int argsRead = sscanf(command, "%s %s", instruction, arg1);
    printf("[DEBUG] command='%s' | instruction='%s' | arg1='%s' |argsRead=%d\n", command,
           instruction, arg1, argsRead);

    for (int j = 0; command[j]; j++) {
        printf("command[%d] = '%c' (%d)\n", j, command[j], command[j]);
    }


    // argumentos variables

    if (strcmp(instruction, "help") == 0) {
        checkArguments(1, argsRead, "help", instruction, arg1);
        help();
    } else if (strcmp(instruction, "zoomin") == 0) {
        zoomIn();
    } else if (strcmp(instruction, "zoomout") == 0) {
        zoomOut();
    } else if (strcmp(instruction, "registers") == 0) {
        getRegs();
    } else if (strcmp(instruction, "clear") == 0) {
        // check_arguments(args_expected, args_read, msg);
        // Lo agregamos para c/u
        clear();
    } else if (strcmp(instruction, "time") == 0) {
        getTime();
    } else if (strcmp(instruction, "divzero") == 0) {
        throwZeroDivisionException();
    } else if (strcmp(instruction, "opcode") == 0) {
        throwInvalidOpcodeException();
    } else if (strcmp(instruction, "pongis") == 0) {
        startGame();
    } else if (isEmpty(instruction)) {
        return;
    } else {
        invalidCommand();
    }
}

int isEmpty(char *command) {
    while (*command == ' ' || *command == '\t') { command++; }
    return *command == '\0';
}

// cuando ingresa un comando no valido
void invalidCommand() {
    puts(INVALID_COMMAND_MESSAGE);
    putLineInBuffer(INVALID_COMMAND_MESSAGE, 0);
}

/* Commands */

void help() {
    char *helpMessages[] = {"Available commands:",
                            "clear: clear terminal.",
                            "divzero: prompts zero division exception.",
                            "help: lists all available commands.",
                            "opcode: prompts invalid operation code exception.",
                            "pongis: starts pongis game.",
                            "registers: lists saved registers.",
                            "time: displays current time.",
                            "zoomin: zooms in text on the screen.",
                            "zoomout: zooms out text on the screen.",
                            "echo %s: echoes text input."};

    int elems = sizeof(helpMessages) / sizeof(char *);

    for (int i = 0; i < elems; i++) {
        puts(helpMessages[i]);
        putLineInBuffer(helpMessages[i], 0);
    }
}

/*
    cuando nos llegue zoom in
    clearScreen();
    for(int i = 0; i < lines; i++){
        puts(Promtp);
        puts(lineBuffer[i].characters);
    }
*/

void zoomIn() {
}

void zoomOut() {
}

void getRegs() {
    register_set_t regs;
    if (sys_get_regs(&regs)) {
        printRegs(regs);
    } else {
        char *registerErrorMsg =
            "Registers must be saved before printing. To save press 'F1'.";
        puts(registerErrorMsg);
        putLineInBuffer(registerErrorMsg, 0);
    }
    return;
}

void printRegs(register_set_t regs) {
    printReg("RAX", regs.rax);
    printReg("RBX", regs.rbx);
    printReg("RCX", regs.rcx);
    printReg("RDX", regs.rdx);
    printReg("RSI", regs.rsi);
    printReg("RDI", regs.rdi);
    printReg("RSP", regs.rsp);
    printReg("RBP", regs.rbp);
    printReg("R8", regs.r8);
    printReg("R9", regs.r9);
    printReg("R10", regs.r10);
    printReg("R11", regs.r11);
    printReg("R12", regs.r12);
    printReg("R13", regs.r13);
    printReg("R14", regs.r14);
    printReg("R15", regs.r15);
    printReg("RFLAGS", regs.rflags);
    printReg("RIP", regs.rip);
    return;
}

void printReg(char *regName, int64_t value) {
    char buffer[25];
    sprintf(buffer, "%s: %x", regName, value);
    puts(buffer);
    putLineInBuffer(buffer, 0);
}

void clear() {
    sys_clear_screen();
    return;
}

// nos piden la hora del sistema
void getTime() {
    time_t time;
    sys_get_time(&time);
    char buffer[50];
    sprintf(buffer, "Local time: %d/%d/%d %d:%d:%d", time.day, time.month, time.year,
            time.hours, time.minutes, time.seconds);
    puts(buffer);
    putLineInBuffer(buffer, 0);
    return;
}

void putLineInBuffer(char *line, int isCommand) {
    strcpy(lineBuffer[lastLine].characters, line);
    lineBuffer[lastLine].isCommand = isCommand;
    lastLine = (lastLine + 1) % MAX_LINES_SAVED;
    commandCount += isCommand;
    if (totalLines >= MAX_LINES_SAVED) {
        if (lineBuffer[firstLine].isCommand) { commandCount--; }
        firstLine = (firstLine + 1) % MAX_LINES_SAVED;
    } else {
        totalLines++;
    }
    return;
}

void checkArguments(int argsExpected, int argsRead, char *command, char *instruction,
                    char *arg1) {
    if (argsRead != argsExpected) {
        puts("Unexpected arguments...\n");
        printf("Expected %d but found %d:\n", argsExpected, argsRead);
        printf("%s %s\n", instruction, argsRead == 2 ? arg1 : "");
        printf("Correct use: $ %s %s \n", command, argsExpected == 2 ? "arg1" : "");
    }
}
