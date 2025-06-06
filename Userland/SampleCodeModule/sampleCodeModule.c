/* sampleCodeModule.c */

#include "./include/stdio.h"
#include <pongis.h>
#include <syscalls.h>
//#include "./include/syscalls.h" --> incluido en pongis.h

extern void throw_invalid_opcode(void);
extern void throw_zero_division(void);

char *v = (char *) 0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {

    //    while (1) {

    //         startGame();
    //         return 0;

    //    }

    int64_t fd = 0;

    const char *buf = "Hola mundo?\n";
    int64_t count = 12;

    // Write inicial
    sys_write(fd, buf, count);

    const char *msg = "Ingrese texto: ";
    sys_write(fd, msg, (int64_t) 15);

    // Read
    char buffer[20];
    count = 20;

    int64_t bytes_read = sys_read(fd, buffer, count);

    // Verificar si realmente leyó algo
    const char *msg2 = "\nLeido: ";
    sys_write(fd, msg2, 8);

    // Mostrar lo que se leyó
    fd = 1;// stdout
    sys_write(fd, buffer, bytes_read);

    sys_write(fd, &"\n", 1);

    // if (bytes_read % 2) {
    //     throw_invalid_opcode();
    // } else {
    //     throw_zero_division();
    // }


    shell();

    if (var1 == 0 && var2 == 0) return 0xDEADC0DE;

    return 0xDEADBEEF;
}