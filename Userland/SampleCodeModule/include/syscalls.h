#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H

#define ARROW_DOWN_CODE 17
#define ARROW_UP_CODE 18
#define ARROW_LEFT_CODE 19
#define ARROW_RIGHT_CODE 20

typedef struct time {
    uint64_t seconds;
    uint64_t minutes;
    uint64_t hours;
    uint64_t day;
    uint64_t month;
    uint64_t year;
} time_t;

typedef struct register_set {
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

    uint64_t rflags;// Flags register
    uint64_t rip;   // Instruction pointer
} register_set_t;
// Our syscalls prototypes

int64_t sys_write(int64_t fd, const void *buf, int64_t count);

int64_t sys_read(int64_t fd, void *buf, int64_t count);

int64_t sys_get_regs(register_set_t *registers);

int64_t sys_get_time(time_t *time);

int64_t sys_delete_char(void);

// ##################### sound syscalls ######################

//pending
int64_t sys_start_beep(uint32_t frecuence);

int64_t sys_beep(uint32_t frecuence, int64_t ticks);
//pending
int64_t sys_stop_beep(void);

int64_t sys_sleep(int64_t ticks);

// ##################### video syscalls ######################

int64_t sys_clear_screen(void);

int64_t sys_draw_circle(uint64_t pos_x, uint64_t pos_y, uint64_t radius,
                        uint32_t hexColor);

int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y,
                     uint32_t hexColor);

int64_t sys_draw_pixel(uint64_t pos_x, uint64_t pos_y, uint32_t hexColor);

int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y , uint32_t hexColor);

#endif