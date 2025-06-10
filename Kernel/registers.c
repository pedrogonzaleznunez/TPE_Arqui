#include <registers.h>
#include <videoDriver.h>

register_set_t register_status;
extern uint8_t saved_registers_flag;

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void printReg(char *name, int64_t value);

void printAllRegs() {
    printReg("RAX: ", register_status.rax);
    printReg("RBX: ", register_status.rbx);
    printReg("RCX: ", register_status.rcx);
    printReg("RDX: ", register_status.rdx);
    printReg("RSI: ", register_status.rsi);
    printReg("RDI: ", register_status.rdi);
    printReg("RSP: ", register_status.rsp);
    printReg("RBP: ", register_status.rbp);
    printReg("R8: ", register_status.r8);
    printReg("R9: ", register_status.r9);
    printReg("R10: ", register_status.r10);
    printReg("R11: ", register_status.r11);
    printReg("R12: ", register_status.r12);
    printReg("R13: ", register_status.r13);
    printReg("R14: ", register_status.r14);
    printReg("R15: ", register_status.r15);
    printReg("RFLAGS: ", register_status.rflags);
    printReg("RIP: ", register_status.rip);
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

    regs->rax = register_status.rax;
    regs->rbx = register_status.rbx;
    regs->rcx = register_status.rcx;
    regs->rdx = register_status.rdx;

    regs->rsi = register_status.rsi;
    regs->rdi = register_status.rdi;
    regs->rsp = register_status.rsp;
    regs->rbp = register_status.rbp;

    regs->r8 = register_status.r8;
    regs->r9 = register_status.r9;
    regs->r10 = register_status.r10;
    regs->r11 = register_status.r11;
    regs->r12 = register_status.r12;
    regs->r13 = register_status.r13;
    regs->r14 = register_status.r14;
    regs->r15 = register_status.r15;

    regs->rflags = register_status.rflags;
    regs->rip = register_status.rip;
    return 1;
}
