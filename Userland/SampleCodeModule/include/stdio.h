#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <string.h>
#include <stdint.h>


#define FD_STDIN  0
#define FD_STDOUT 1
#define FD_STDERR 2

// Ademas se debera definir un set de funciones para interactuar con dicha API. Debera
// ser el equivalente en Linux a la biblioteca estandar de C, en la cual se debera basar. Por
// ejemplo, debera contar con funciones como scanf, printf, putChar, getChar.
void vprintf(const char *str, va_list args);
void printf(const char *format, ...);
void fprintf(int fd, const char *str, ...);

int vscanf(const char *format, va_list args);
int vsscanf(const char *buffer, const char *format, va_list args);
int scanf(const char *format, ...);
int sscanf(const char *str, const char *format, ...);

void puts(const char *str);
int getchar();
void putchar(const char c);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);


#endif