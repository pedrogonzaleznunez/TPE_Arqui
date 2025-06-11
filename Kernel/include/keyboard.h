#ifndef KEYBOARD_H
#define KEYBOARD_H

#define SHIFT_RELEASED_LEFT  0xAA
#define SHIFT_RELEASED_RIGHT 0xB6
#define SHIFT_PRESSED_LEFT   0x2A
#define SHIFT_PRESSED_RIGHT  0x36
#define CAPS_LOCK_PRESSED    0x3A
#define F1_REGISTER_BACKUP   0x3B

#define ARROW_DOWN_CODE  17
#define ARROW_UP_CODE    18
#define ARROW_LEFT_CODE  19
#define ARROW_RIGHT_CODE 20

#define BUFFER_DEFAULT_SIZE 1024
#define MAX_SCAN_CODE       0x53

#include <stdint.h>

// imprime el valor dado si es que es imprimible
void printKey(char key);

char getChar(uint8_t scanCode);

// devuelve el scancode de la tecla presionada
char getKey();

void keyboardHandler();

char bufferRead();

int isBufferEmpty();

int isBufferFull();

int64_t getKeyState(uint8_t *target);

#endif
