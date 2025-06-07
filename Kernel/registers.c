#include <registers.h>
#include <videoDriver.h>

register_set_t saved_registers, saved_registers_on_exception;
extern uint8_t saved_registers_flag;

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void printReg(char *name, int64_t value);

void printAllRegs(uint32_t error) {
    register_set_t toPrint = error ? saved_registers_on_exception : saved_registers;
    printReg("RAX: ", toPrint.rax);
    printReg("RBX: ", toPrint.rbx);
    printReg("RCX: ", toPrint.rcx);
    printReg("RDX: ", toPrint.rdx);
    printReg("RSI: ", toPrint.rsi);
    printReg("RDI: ", toPrint.rdi);
    printReg("RSP: ", toPrint.rsp);
    printReg("RBP: ", toPrint.rbp);
    printReg("R8: ", toPrint.r8);
    printReg("R9: ", toPrint.r9);
    printReg("R10: ", toPrint.r10);
    printReg("R11: ", toPrint.r11);
    printReg("R12: ", toPrint.r12);
    printReg("R13: ", toPrint.r13);
    printReg("R14: ", toPrint.r14);
    printReg("R15: ", toPrint.r15);
    printReg("RFLAGS: ", toPrint.rflags);
    printReg("RIP: ", toPrint.rip);
    return;
}

void printReg(char *name, int64_t value) {
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

int64_t setRegisters(register_set_t *regs) {
    if (!saved_registers_flag) { return 0; }

    regs->rax = saved_registers.rax;
    regs->rbx = saved_registers.rbx;
    regs->rcx = saved_registers.rcx;
    regs->rdx = saved_registers.rdx;

    regs->rsi = saved_registers.rsi;
    regs->rdi = saved_registers.rdi;
    regs->rsp = saved_registers.rsp;
    regs->rbp = saved_registers.rbp;

    regs->r8 = saved_registers.r8;
    regs->r9 = saved_registers.r9;
    regs->r10 = saved_registers.r10;
    regs->r11 = saved_registers.r11;
    regs->r12 = saved_registers.r12;
    regs->r13 = saved_registers.r13;
    regs->r14 = saved_registers.r14;
    regs->r15 = saved_registers.r15;

    regs->rflags = saved_registers.rflags;
    regs->rip = saved_registers.rip;
    return 1;
}
