#include <naiveConsole.h>
#include <registers.h>
#include <videoDriver.h>

register_set_t register_status;
extern uint8_t saved_registers_flag;

void printReg(char *name, int64_t value, int32_t color);

void printAllRegs(int32_t color) {
    printReg("RAX: ", register_status.rax, color);
    printReg("RBX: ", register_status.rbx, color);
    printReg("RCX: ", register_status.rcx, color);
    printReg("RDX: ", register_status.rdx, color);
    printReg("RSI: ", register_status.rsi, color);
    printReg("RDI: ", register_status.rdi, color);
    printReg("RSP: ", register_status.rsp, color);
    printReg("RBP: ", register_status.rbp, color);
    printReg("R8: ", register_status.r8, color);
    printReg("R9: ", register_status.r9, color);
    printReg("R10: ", register_status.r10, color);
    printReg("R11: ", register_status.r11, color);
    printReg("R12: ", register_status.r12, color);
    printReg("R13: ", register_status.r13, color);
    printReg("R14: ", register_status.r14, color);
    printReg("R15: ", register_status.r15, color);
    printReg("RFLAGS: ", register_status.rflags, color);
    printReg("RIP: ", register_status.rip, color);
    return;
}

void printReg(char *name, int64_t value, int32_t color) {
    char numBuffer[20] = {0};
    putString(name, color);
    uintToBase(value, numBuffer, 16);
    putString(numBuffer, color);
    putChar('\n', color);
    return;
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
