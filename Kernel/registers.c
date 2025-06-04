#include <registers.h>
#include <videoDriver.h>

register_set_t saved_registers;
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void printReg(char * name, int64_t value);

void regs_print(void){
    printReg("RAX: ", saved_registers.rax);
    printReg("RBX: ", saved_registers.rbx);
    printReg("RCX: ", saved_registers.rcx);
    printReg("RDX: ", saved_registers.rdx);
    printReg("RSI: ", saved_registers.rsi);
    printReg("RDI: ", saved_registers.rdi);
    printReg("RSP: ", saved_registers.rsp);
    printReg("RBP: ", saved_registers.rbp);
    printReg("R8: ", saved_registers.r8);
    printReg("R9: ", saved_registers.r9);
    printReg("R10: ", saved_registers.r10);
    printReg("R11: ", saved_registers.r11);
    printReg("R12: ", saved_registers.r12);
    printReg("R13: ", saved_registers.r13);
    printReg("R14: ", saved_registers.r14);
    printReg("R15: ", saved_registers.r15);
    printReg("RFLAGS: ", saved_registers.rflags);
    printReg("RIP: ", saved_registers.rip);
    return;
}

void printReg(char * name, int64_t value){
    char numBuffer[20] = {0};
    putString(name, 0x00FF00);
    uintToBase(value, numBuffer, 16);
    putString(numBuffer, 0x00FF00);
    putChar('\n', 0x00FF00);
    return;
}

// Esto es aux
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    // Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
