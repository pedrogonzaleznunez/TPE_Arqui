#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void putCharInPos(char c, uint32_t hexColor, uint64_t x, uint64_t y);

void putStringInPos(char* str, uint32_t hexColor, uint64_t x, uint64_t y);

void newLine();

void putString(char *str, uint32_t hexColor);

void putChar(char c, uint32_t hexColor);

#endif
