#include <shell.h>
#include <syscalls.h>

void shell(){
    puts(WELCOME_MESSAGE);
    help();
      int64_t fd = 0;

    while(1){
        // const char *msg = "activar shell ";
        // sys_write(fd, msg, (int64_t) 15);
        activate_shell();
    }
    
}

void activate_shell(){
    putchar(PROMPT_SYMBOL);
    char shell_buffer[MAX_COMMAND_LENGTH] = {0};

    int64_t bytes_read = sys_read(0, shell_buffer, MAX_COMMAND_LENGTH);

    //int i = 0;
    // char c;
    // while((c = getchar()) != '\n' && i < MAX_COMMAND_LENGTH - 1){
    //     // manejar back space
    //     if( c >= 32 && c <= 126){
    //         shell_buffer[i++] = c;
    //         putchar(c);
    //     } 
    // }

    //shell_buffer[i] = '\0';
    //puts("Termino de leer\n");

    if(bytes_read == MAX_COMMAND_LENGTH){
        shell_buffer[bytes_read - 1 ] = 0;
    } 

    process_commands(shell_buffer);
    

    // manejar borrado desde aca 



}

// ojo que cuando borro ya no me lo toma como valido 

void process_commands(char * command){
    
    if(strcmp(command, "help") == 0){
        help();
    } else if(strcmp(command, "zoomin") == 0 ){
        zoom_in();
    } else if(strcmp(command, "zoomout") == 0 ){
        zoom_out();
    } else if(strcmp(command, "registers") == 0 ){
        get_regs();
    } else if(strcmp(command, "clear") == 0 ){
        clear();
    } else if(strcmp(command, "time") == 0 ){
        get_time();
    } else if(strcmp(command, "divzero") == 0 ){
        invalid_command();
    } else if(strcmp(command, "opcode") == 0 ){
        invalid_command(); 
    } else if(strcmp(command, "pongis") == 0 ){
        invalid_command();
    } else if(is_empty(command)) {
        return;
    } else {
        invalid_command();
    }
        
}

int is_empty(char * command){
    while(*command == ' ' || *command == '\t'){
        command++;
    }
    return *command == '\0';
}

// cuando ingresa un comando no valido
void invalid_command(){
    puts(INVALID_COMMAND_MESSAGE);
    //help();
}


/* Commands */
    

void help(){
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

void zoom_in(){

}

void zoom_out(){

}
    
void get_regs(){
    register_set_t regs;
    puts("\nsys call que imprime:\n");
    sys_print_regs();
    puts("\nsys call que los carga\n");
    sys_get_regs(&regs);
    print_regs(regs);

}


void print_regs(register_set_t regs) {
    printReg("RAX: ", regs.rax);
    printReg("RBX: ", regs.rbx);
    printReg("RCX: ", regs.rcx);
    printReg("RDX: ", regs.rdx);
    printReg("RSI: ", regs.rsi);
    printReg("RDI: ", regs.rdi);
    printReg("RSP: ", regs.rsp);
    printReg("RBP: ", regs.rbp);
    printReg("R8: ", regs.r8);
    printReg("R9: ", regs.r9);
    printReg("R10: ", regs.r10);
    printReg("R11: ", regs.r11);
    printReg("R12: ", regs.r12);
    printReg("R13: ", regs.r13);
    printReg("R14: ", regs.r14);
    printReg("R15: ", regs.r15);
    printReg("RFLAGS: ", regs.rflags);
    printReg("RIP: ", regs.rip);
    return;
}

void printReg(char *name, int64_t value) {
    char numBuffer[20] = {0};
    puts(name);
    uintToBase(value, numBuffer, 16);
    puts(numBuffer);
    return;
}
















void clear(){

}

// nos piden la hora del sistema
void get_time(){
    time_t time;
    sys_get_time(&time);
    printf("Local time: %d/%d/%d %d:%d:%d ", time.day, time.month, time.year, time.hours, time.minutes, time.seconds);
    putchar('\n');
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
    
    