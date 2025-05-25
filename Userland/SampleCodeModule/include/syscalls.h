#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <stdarg.h>

// int32_t sys_call(int64_t fd, const void * buf, int64_t count, int sysId); algo asi pero argumentos variables

int32_t sys_call(int64_t id, ...);

// poner en el manual para que el usuario sepa con que argumentos debe llamar a cada syscall ?
// Our syscalls prototypes

// int32_t sys_write(int64_t fd, const void * buf, int64_t count);
// int32_t sys_read(int64_t fd, void * buf, int64_t count);
// int32_t sys_start_beep(uint32_t frecuence);
// int32_t sys_stop_beep(void);

#endif