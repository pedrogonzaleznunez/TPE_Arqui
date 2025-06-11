#ifndef __FONT_H_
#define __FONT_H_

#include <stddef.h>
#include <stdint.h>

#define CHAR_U_UMLAUT_OFFSET 95 // Adjust based on your fontBitmap size
#define MIN_CHAR ' '
#define MAX_CHAR '~'

#define DEFAULT_WIDTH  8
#define DEFAULT_HEIGHT 16

uint8_t getFontWidth();

uint8_t getFontHeight();

uint8_t *getFontChar(char c);

#endif