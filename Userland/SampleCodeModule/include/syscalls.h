#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdarg.h>
#include <stdint.h>

// Our syscalls prototypes

int64_t sys_write(int64_t fd, const void * buf, int64_t count);

int64_t sys_read(int64_t fd, void * buf, int64_t count);

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

#endif