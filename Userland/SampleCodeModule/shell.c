#include <shell.h>
#include <syscalls.h>
#include <myStrings.h>

#define printReg(x, y) printf("%s: %x\n", x, y)

#define MAX_LINES_SAVED 40

typedef struct line{
    char characters[MAX_COMMAND_LENGTH];
    int length;
    int isCommand;
} line_t;

static line_t lineBuffer[MAX_LINES_SAVED];
static firstLine = 0;
static lastLine = 0;

/*
    cuando nos llegue zoom in
    clearScreen();
    for(int i = 0; i < lines; i++){
        puts(Promtp);
        puts(lineBuffer[i].characters);
    }
*/

void shell() {
    puts(WELCOME_MESSAGE);
    help();
    int64_t fd = 0;

    firstLine = 0;
    lastLine = 0;

    while (1) {
        // const char *msg = "activar shell ";
        // sys_write(fd, msg, (int64_t) 15);
        activate_shell();
    }
}

void activate_shell() {
    printf("%s ", PROMPT_SYMBOL);

    char shell_buffer[MAX_COMMAND_LENGTH] = {0};

    int i = 0;
    char c;
    int read = 1;

    while (read && i < MAX_COMMAND_LENGTH) {
        int64_t ret = sys_read(0, &c, 1);// leer un carácter

        //if(ret <= 0) continue;

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
        } else {
            //if(i < MAX_COMMAND_LENGTH - 1){
            shell_buffer[i++] = c;
            putchar(c); // mostrar en pantalla
            // }
        }
        // ignorar otros caracteres
    }

    shell_buffer[i] = '\0';
    if (i == MAX_COMMAND_LENGTH) { 
        shell_buffer[i - 1] = 0; 
        putchar('\n');
    }

    // acá terminó de leer una línea ->
    strcpy(lineBuffer[lastLine].characters, PROMPT_SYMBOL);
    lineBuffer[lastLine].length = strlen(PROMPT_SYMBOL);
    strcat(lineBuffer[lastLine].characters, shell_buffer);
    lineBuffer[lastLine].length += i;

    lastLine = (lastLine + 1) % MAX_LINES_SAVED;
    firstLine += (lastLine == firstLine);

    process_commands(shell_buffer);
}


// ojo que cuando borro ya no me lo toma como valido

void process_commands(char *command) {
    // agregar función trim 
    // devuelve un puntero a char con la primera palabra y la borra del que mandaste
    // nada, nada, deja, deja.
    // se puede hacer con sscanf
    char instruction[MAX_COMMAND_LENGTH]; // vemos long
    int arg1;

    trim(command);   

    int args_read = sscanf(command, "%s %d", &instruction, &arg1);

    // argumentos variables 
    
    if (strcmp(instruction, "help") == 0) {
        if(args_read != 1){
            // "unexpected arguments..."
            // "expected %d, found %d", args_expected, args_read - 1.
            // "correct use: $ %s", msg
        }
        help();
    } else if (strcmp(instruction, "zoomin") == 0) {
        zoom_in();
    } else if (strcmp(instruction, "zoomout") == 0) {
        zoom_out();
    } else if (strcmp(instruction, "registers") == 0) {
        get_regs();
    } else if (strcmp(instruction, "clear") == 0) {
        // check_arguments(args_expected, args_read, msg);
        // Lo agregamos para c/u
        clear();
    } else if (strcmp(instruction, "time") == 0) {
        get_time();
    } else if (strcmp(instruction, "divzero") == 0) {
        invalid_command();
    } else if (strcmp(instruction, "opcode") == 0) {
        invalid_command();
    } else if (strcmp(instruction, "pongis") == 0) {
        invalid_command();
    } else if (is_empty(instruction)) {
        return;
    } else {
        invalid_command();
    }
}

int is_empty(char *command) {
    while (*command == ' ' || *command == '\t') { command++; }
    return *command == '\0';
}

// cuando ingresa un comando no valido
void invalid_command() {
    puts(INVALID_COMMAND_MESSAGE);
    //help();
}


/* Commands */


void help() {
    puts("\nAvailable commands:\n");
    puts("\nclear: clear terminal.");
    puts("\ndivzero: prompts zero division exception.");
    puts("\nhelp: lists all available commands.");
    puts("\nopcode: prompts invalid operation code exception.");
    puts("\npongis: starts pongis game.");
    puts("\nregisters: lists saved registers.");
    puts("\ntime: displays current time.");
    puts("\nzoomin: zooms in text on the screen.");
    puts("\nzoomout: zooms out text on the screen.");
}

void zoom_in() {
}

void zoom_out() {
}

void get_regs() {
    register_set_t regs;
    // puts("\nsys call que imprime:\n");   // Doy de baja la syscall que los imprime directamente
    // sys_print_regs();
    // puts("\nsys call que los carga\n");
    if (sys_get_regs(&regs)) {
        print_regs(regs);
    } else {
        puts("Registers must be saved before printing. To save press 'q'");
    }
}

void print_regs(register_set_t regs) {
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

void clear() {
}

// nos piden la hora del sistema
void get_time() {
    time_t time;
    sys_get_time(&time);
    printf("Local time: %d/%d/%d %d:%d:%d\n", time.day, time.month, time.year, time.hours,
           time.minutes, time.seconds);
    return;
}

