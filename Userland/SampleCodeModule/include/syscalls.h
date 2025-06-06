#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H


typedef struct time {
    uint64_t seconds;
    uint64_t minutes;
    uint64_t hours;
    uint64_t day;
    uint64_t month;
    uint64_t year;
} time_t;

// Our syscalls prototypes

int64_t sys_write(int64_t fd, const void * buf, int64_t count);

int64_t sys_read(int64_t fd, void * buf, int64_t count);

int64_t sys_print_regs(void);

int64_t sys_get_time(time_t * time);

// ##################### sound syscalls ######################

//pending
int64_t sys_start_beep(uint32_t frecuence);

int64_t sys_beep(uint32_t frecuence, int64_t ticks);
//pending
int64_t sys_stop_beep(void);

int64_t sys_sleep(int64_t ticks);

// ##################### video syscalls ######################

int64_t sys_clear_screen(void);

int64_t sys_draw_circle(uint64_t pos_x, uint64_t pos_y, uint64_t radius, uint32_t hexColor);

int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y , uint32_t hexColor);

int64_t sys_draw_pixel(uint64_t pos_x, uint64_t pos_y, uint32_t hexColor);

#endif