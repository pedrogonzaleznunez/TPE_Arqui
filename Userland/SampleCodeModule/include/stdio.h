#ifndef _STDIO_H_
#define _STDIO_H_

#include <myStrings.h>
#include <stdarg.h>
#include <stdint.h>

#define FD_STDIN  0
#define FD_STDOUT 1
#define FD_STDERR 2

void vprintf(const char *str, va_list args);
void printf(const char *format, ...);
void fprintf(int fd, const char *str, ...);
void sprintf(char *str, const char *format, ...);

int vscanf(const char *format, va_list args);
int vsscanf(const char *buffer, const char *format, va_list args);
int scanf(const char *format, ...);
int sscanf(const char *str, const char *format, ...);

void puts(const char *str);
int getchar();
void putchar(const char c);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

void putsCenter(const char *str);

void printPadding(char c, int count);
void putsWithPadding(const char *str, int pad_top, int pad_left);
void putsWidthCenter(const char *str);
void putsHeightFourthCenter(const char *str);
void putsCenterWidthHeightFourthCenter(const char *str);

#endif /* _STDIO_H_ */
