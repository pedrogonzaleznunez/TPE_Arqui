/* sampleCodeModule.c */

#include "./include/stdio.h"
#include "./include/syscalls.h"

char *v = (char *) 0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {

    // printf("[TEST] for printf\n");
    // printf("next line");
    // puts("\n");
    // puts("[TEST] for puts");
    // puts("[TEST] for putchar A,B,C\n");
    // putchar('A');
    // putchar('B');
    // putchar('C');

    // test to draw a circle
    
    sys_draw_circle(100, 100, 50, 0xFF0000);

    // int64_t fd = 0;

    // const char *buf = "Hola mundo?\n";
    // int64_t count = 12;

    // // Write inicial
    // sys_call(0, fd, buf, count);

    // const char *msg = "Ingrese texto: ";
    // sys_call(0, fd, msg, (int64_t) 15);

    // // Read
    // char buffer[20];
    // count = 20;

    // int64_t bytes_read = sys_call(1, fd, buffer, count);

    // // Verificar si realmente leyó algo
    // const char *msg2 = "\nLeido: ";
    // sys_call(0, fd, msg2, 8);

    // // Mostrar lo que se leyó
    // fd = 1;// stdout
    // sys_call(0, fd, buffer, bytes_read);

    // Test if BSS is properly set up
    if (var1 == 0 && var2 == 0) return 0xDEADC0DE;

    return 0xDEADBEEF;
}