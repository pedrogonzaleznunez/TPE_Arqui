#include <shell.h>
#include <stdio.h>
#include <string.h>


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
    char shell_buffer[MAX_COMMAND_LENGTH];

    int64_t bytes_read = sys_read(0, shell_buffer, MAX_COMMAND_LENGTH);

     int i = 0;
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

    process_commands(shell_buffer, i);
    
}

// ojo que cuando borro ya no me lo toma como valido 

void process_commands(char * command, int length){

    if(strcmp(command, "help") == 0){
        help();
    } else {
        invalid_command();
    }
        
}

// cuando ingresa un comando no valido
void invalid_command(){
    puts(INVALID_COMMAND_MESSAGE);
    help();
}

// cuando solo ingresa espacios, tabs o nada
void response_to_no_command();

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

}

void clear(){

}

// nos piden la hora del sistema
void get_time(){

}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
    
    