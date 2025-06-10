#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

#define DEFAULT_BGR_COLOR  0x000000
#define DEFAULT_CHAR_COLOR 0xFFFFFF
#define ERROR_CHAR_COLOR   0xFF0000

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void putCharInPos(char c, uint32_t hexColor, uint64_t x, uint64_t y);

void putStringInPos(char *str, uint32_t hexColor, uint64_t x, uint64_t y);

void newLine();

void putString(char *str, uint32_t hexColor);

void putChar(char c, uint32_t hexColor);

void clearScreen();

void deleteChar();

void drawCircle(uint64_t pos_x, uint64_t pos_y, uint64_t radius, uint32_t hexColor);

void drawRec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y,
             uint32_t hexColor);

void fillScreen(uint32_t hexColor);

void setMultiplier(int more);

int getWidthInChars(void);
#endif
