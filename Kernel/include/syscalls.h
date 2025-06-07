#ifndef __SYSCALLS__H_
#define __SYSCALLS__H_

#include <registers.h>
#include <rtc.h>
#include <stdint.h>

void _syscallHandler(void);

int64_t write(int64_t fd, const char *buf, int64_t count);// write video
int64_t read(int64_t fd, char *buf, int64_t count);       // read
int64_t sys_beep(uint32_t frecuency, int64_t ticks);

// estas solo llaman a otra funcion del kernel -> volar 
// int64_t sys_print_regs(void); que no haya syscall, que los obtenga y después decida cómo imprimir.
//int64_t get_regs(register_set_t *registers);
//int64_t sys_get_time(time_t *time);
//int64_t sys_start_beep(uint32_t frecuency);
//int64_t sys_stop_beep(void);
//int64_t sys_sleep(int64_t ticks);
//int64_t sys_clear_screen(void);
//int64_t sys_draw_circle(uint64_t pos_x, uint64_t pos_y, uint64_t radius, uint32_t hexColor);
//int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y, uint32_t hexColor);
//int64_t sys_fill_screen(uint32_t hexColor);
//int64_t sys_draw_pixel(uint64_t pos_x, uint64_t pos_y, uint32_t hexColor);

#endif
