#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct register_set{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rsp;
    uint64_t rbp;
    
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    // Flags register
    uint64_t rflags;

    // Instruction pointer
    uint64_t rip;
} register_set_t;

extern register_set_t saved_registers;
extern uint8_t save_registers_flag;


#endif /* REGISTERS_H */