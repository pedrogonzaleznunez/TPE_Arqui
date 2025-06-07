#include <interrupts.h>
#include <keyboard.h>
#include <registers.h>
#include <syscalls.h>
#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
    switch (exception) {
        case ZERO_EXCEPTION_ID:
            zero_division();
            break;
        case INVALID_OPCODE_ID:
            invalid_opcode();
            break;
    }
    return;
}

void exceptionHelper(char *msg) {
    putString(msg, 0xFF0000);
    putString("Registers at exception show: \n", 0xFF0000);
    printAllRegs(1);// 1 para que imprima en error

    while (!isBufferEmpty()) { bufferRead(); }// vacío el buffer

    char c = 0;
    putString("\nPress enter to return to main\n", 0xFF0000);
    // Volver a habilitar interrupciones
    _sti();
    while (c != '\n') { read(1, &c, 1); }
    clearScreen();
    return;
}

static void zero_division() {
    // Handler para manejar excepcíon
    exceptionHelper("Zero division exception\n");
    return;
}

static void invalid_opcode() {
    // Handler para manejar excepcíon
    exceptionHelper("Invalid opcode exception\n");
    return;
}
