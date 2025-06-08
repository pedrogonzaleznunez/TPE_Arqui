#ifndef __MY_STRINGS_H_
#define __MY_STRINGS_H_

// Devuelve la longitud de una cadena de caracteres
int strlen(const char *s);

// Copia a t todos los caracteres de s
char * strcpy(char * t, const char * s);

// Copia a t los primeros n caracteres de s
char * strncpy(char * t, const char * s, unsigned int n);

// Concatena a t todos los caracteres de s
char * strcat(char * t, const char * s);

// Concatena a t los primeros n caracteres de s
char * strncat(char * t, const char * s, int n);

// Devuelve 0 si las cadenas son iguales, 1 si t es mayor y -1 si s es mayor
int strcmp(const char *t, const char *s);

// Compara los primeros n caracteres de ambas cadenas. Devuelve 0 si las cadenas son iguales, 1 si t es mayor y -1 si s es mayor
int strncmp(const char * t, const char * s, unsigned int n);

// Devuelve el puntero a la primera aparición de c en la cadena
char * strchr(const char * s, char c);

// Devuelve el puntero a la última aparición de c en la cadena
char * strrchr(const char * s, char c);

void trim(char * str);

#endif
