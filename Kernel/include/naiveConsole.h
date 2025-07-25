#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char *string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

void ncPrintColor(const char *string, const int format);
void ncPrintCharColor(char character, const int format);

void ncDeleteChar(char amount);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

#endif /* NAIVE_CONSOLE_H */
