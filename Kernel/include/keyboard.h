#ifndef KEYBOARD_H
#define KEYBOARD_H

// espera a que le llegue una tecla y la imprime
void printPressedKey();

// imprime el valor dado si es que es imprimible
void printKey(char key);

char getChar(char scanCode);

// devuelve el valor de la tecla presionada
char getKey();

void keyboardHandler();

#endif
