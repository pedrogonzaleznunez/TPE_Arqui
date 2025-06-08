#ifndef _SOUNDS_H__
#define _SOUNDS_H__

#include <stdint.h>

void playSound(uint32_t nFrequence);

void noSound();

void beep();

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

#endif
