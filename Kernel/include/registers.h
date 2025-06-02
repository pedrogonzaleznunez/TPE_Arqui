#include <stdint.h>

#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct register_set{
    uint64_t rax; // 1
    uint64_t rbx; // 2
    uint64_t rcx; // 3
    uint64_t rdx; // 4

    uint64_t rsi; // 5
    uint64_t rdi; // 6
    uint64_t rsp; // 7
    uint64_t rbp; // 8
    
    uint64_t r8; // 9
    uint64_t r9;
    uint64_t r10;
    uint64_t r11; //12
    uint64_t r12; //13
    uint64_t r13;   // 14
    uint64_t r14; // 15
    uint64_t r15; // 16

    // Flags register
    uint64_t rflags; // 17

    // Instruction pointer
    uint64_t rip; // 18
} register_set_t;

// extern register_set_t saved_registers;
// extern uint8_t save_registers_flag;


#endif /* REGISTERS_H */