#ifndef KEYBOARD_H
#define KEYBOARD_H

#define SHIFT_RELEASED_LEFT 0xAA
#define SHIFT_RELEASED_RIGHT 0xB6
#define SHIFT_PRESSED_LEFT 0x2A
#define SHIFT_PRESSED_RIGHT 0x36
#define CAPS_LOCK_PRESSED 0x3A
#define F1_REGISTER_BACKUP 0x3B

// imprime el valor dado si es que es imprimible
void printKey(char key);

char getChar(char scanCode);

// devuelve el scancode de la tecla presionada
char getKey();

void keyboardHandler();

char bufferRead();

int isBufferEmpty();

int isBufferFull();

#endif
