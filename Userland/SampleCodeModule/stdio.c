#include "include/stdio.h"
#include "include/syscalls.h"
#include <stdarg.h>
#include <stdint.h>

// todo manejar colores ??

static char buffer[64] = {0};

static void printBase(int fd, int num, int base);
void vfprintf(int fd, const char *format, va_list args);
void skip_whitespace(const char *buffer, int *idx);
//static
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

void puts(const char *str) {
    while (*str) { putchar(*str++); }
    putchar('\n');
}

void printPadding(char c, int count) {
    for (int i = 0; i < count; i++) { putchar(c); }
}

void putsWithPadding(const char *str, int pad_top, int pad_left) {
    printPadding('\n', pad_top);
    printPadding(' ', pad_left);
    puts(str);
}

void putsWidthCenter(const char *str) {
    int screen_width = sys_chars_width();
    int pad_left = (screen_width - strlen(str)) / 2;
    putsWithPadding(str, 0, pad_left);
}

void putsHeightFourthCenter(const char *str) {
    int screen_height = sys_chars_height();
    int pad_top = screen_height / 4;
    putsWithPadding(str, pad_top, 0);
}

void putsCenterWidthHeightFourthCenter(const char *str) {
    int screen_width = sys_chars_width();
    int screen_height = sys_chars_height();
    int pad_left = (screen_width - strlen(str)) / 2;
    int pad_top = screen_height / 4;
    putsWithPadding(str, pad_top, pad_left);
}

void putsCenter(const char *str) {
    int screen_width = sys_chars_width();
    int screen_height = sys_chars_height();
    int pad_left = (screen_width - strlen(str)) / 2;
    int pad_top = screen_height / 2;
    putsWithPadding(str, pad_top - 1, pad_left);
}

void putchar(const char c) {
    sys_write(FD_STDOUT, &c, 1);// syscall para escribir un caracter
}

int getchar() {
    char c;
    sys_read(FD_STDIN, &c, 1);// syscall para leer un caracter
    return c;
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    // tira error, pero compila bien.
    // es porque quiere encontrar la cte FD_STDOUT en la lib estandar posta
    vfprintf(FD_STDOUT, format, args);
    va_end(args);
}

void fprintf(int fd, const char *str, ...) {
    va_list args;
    va_start(args, str);
    vfprintf(fd, str, args);
    va_end(args);
}

void vfprintf(int fd, const char *format, va_list args) {

    int i = 0;

    while (format[i] != 0) {

        // Verificamos si tenemos un caracter de formato
        switch (format[i]) {

            case '%':
                i++;
                switch (format[i]) {
                    case 'x':
                        printBase(fd, va_arg(args, int), 16);
                        break;
                    case 'd':
                        printBase(fd, va_arg(args, int), 10);
                        break;
                    case 'o':
                        printBase(fd, va_arg(args, int), 8);
                        break;
                    case 'b':
                        printBase(fd, va_arg(args, int), 2);
                        break;
                    // case 'f': printFloat(fd, va_arg(args, double)); break ;
                    case 'c': {
                        char c = (char) va_arg(args, int);
                        sys_write(fd, &c, 1);// syscall para escribir un caracter
                        break;
                    }
                    case 's':
                        fprintf(fd, va_arg(args, char *));
                        break;
                    case '%':
                        sys_write(fd, &"%", 1);// syscall para escribir un caracter
                        break;
                }
                i++;
                break;

            // Si no es un caracter de formato, lo imprimimos directamente
            default:
                sys_write(fd, &format[i], 1);
                i++;
                break;
        }
    }
}

int vscanf(const char *format, va_list args) {
    char buffer[4096];
    char c;
    uint32_t bytes_read = 0;

    while ((c = getchar()) != '\n' && c && bytes_read < sizeof(buffer)) {
        buffer[bytes_read++] = c;
    }

    buffer[bytes_read] = 0;

    return vsscanf(buffer, format, args);
}

void skip_whitespace(const char *buffer, int *idx) {
    while (buffer[*idx] == ' ' || buffer[*idx] == '\t' || buffer[*idx] == '\n' ||
           buffer[*idx] == '\r') {
        (*idx)++;
    }
}

int vsscanf(const char *buffer, const char *format, va_list args) {
    int form_i = 0;
    int buf_i = 0;
    int args_read = 0;

    while (format[form_i] != '\0') {
        // si hay un modificador:
        if (format[form_i] == '%') {
            form_i++;// omitimos el símbolo
            switch (format[form_i]) {
                case 'd': {
                    skip_whitespace(buffer, &buf_i);// saltar espacios antes del número

                    if (buffer[buf_i] == '\0') break;// fin de buffer

                    int num = 0;
                    uint8_t negative = 0, read_num = 0;

                    // signo
                    if (buffer[buf_i] == '-') {
                        negative = 1;
                        buf_i++;
                    } else if (buffer[buf_i] == '+') {
                        buf_i++;
                    }

                    // tomar los dígitos
                    while (buffer[buf_i] >= '0' && buffer[buf_i] <= '9' &&
                           buffer[buf_i] != '\0') {
                        num = num * 10 + buffer[buf_i] - '0';
                        buf_i++;
                        read_num++;
                    }

                    if (read_num > 0) {
                        *va_arg(args, int *) = num * (negative ? -1 : 1);
                        args_read++;
                    } else {
                        return args_read;// no se leyó el número. Termina
                    }
                    break;
                }
                case 'c': {
                    // %c no salta whitespace
                    if (buffer[buf_i] == '\0') break;
                    *va_arg(args, char *) = buffer[buf_i++];
                    args_read++;
                    break;
                }
                case 's': {
                    // saltar espacios antes del string
                    skip_whitespace(buffer, &buf_i);

                    if (buffer[buf_i] == '\0') break;

                    char *str = va_arg(args, char *);
                    int chars_read = 0;

                    // leer hasta whitespace o fin de buffer
                    while (buffer[buf_i] != ' ' && buffer[buf_i] != '\t' &&
                           buffer[buf_i] != '\n' && buffer[buf_i] != '\r' &&
                           buffer[buf_i] != '\0') {
                        *str = buffer[buf_i++];
                        str++;
                        chars_read++;
                    }

                    *str = '\0';

                    if (chars_read > 0) {
                        args_read++;
                    } else {
                        return args_read;// Termina
                    }
                    break;
                }

                default:
                    // especificador no reconocido. Termina
                    return args_read;
            }

            form_i++;// avanzar después del especificador

        } else {
            // debe coincidir exactamente
            if (format[form_i] == ' ') {
                skip_whitespace(buffer, &buf_i);// un espacio equivale a cualquier espacio
            } else {
                if (buffer[buf_i] != format[form_i]) {
                    return args_read;// no coincide. Termina.
                }
                buf_i++;
            }
            form_i++;
        }
    }

    return args_read;
}

int sscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int aux = vsscanf(str, format, args);
    va_end(args);
    return aux;
}

int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int aux = vscanf(format, args);
    va_end(args);
    return aux;
}

static void printBase(int fd, int num, int base) {
    if (num < 0) fprintf(fd, "-");
    uintToBase(num, buffer, base);
    fprintf(fd, buffer);
}

//static
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
    char *p = buffer;
    char digits[] = "0123456789ABCDEF";
    uint32_t length = 0;

    if (base < 2 || base > 16) {
        *p = 0;
        return 0;
    }

    // Handle 0 explicitly
    if (value == 0) {
        *p++ = '0';
        *p = 0;
        return 1;
    }

    char temp[65];
    int i = 0;

    while (value > 0) {
        temp[i++] = digits[value % base];
        value /= base;
    }

    length = i;

    // Reverse the string into buffer
    while (i > 0) { *p++ = temp[--i]; }
    *p = 0;

    return length;
}

void sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int i = 0;
    int a = 0;

    while (format[i] != 0) {

        // Verificamos si tenemos un caracter de formato
        switch (format[i]) {

            case '%':
                i++;
                switch (format[i]) {
                    case 'x':
                        uintToBase(va_arg(args, int), buffer, 16);
                        strcpy(str + a, buffer);
                        a += strlen(buffer);
                        break;
                    case 'd':
                        uintToBase(va_arg(args, int), buffer, 10);
                        strcpy(str + a, buffer);
                        a += strlen(buffer);
                        break;
                    case 'o':
                        uintToBase(va_arg(args, int), buffer, 8);
                        strcpy(str + a, buffer);
                        a += strlen(buffer);
                        break;
                    case 'b':
                        uintToBase(va_arg(args, int), buffer, 2);
                        strcpy(str + a, buffer);
                        a += strlen(buffer);
                        break;
                    // case 'f': printFloat(fd, va_arg(args, double)); break ;
                    case 'c': {
                        char c = (char) va_arg(args, int);
                        str[a++] = c;
                        break;
                    }
                    case 's':
                        char *toAdd = va_arg(args, char *);
                        strcpy(str + a, toAdd);
                        a += strlen(toAdd);
                        break;
                    case '%':
                        str[a++] = '%';
                        break;
                }
                i++;
                break;

            // Si no es un caracter de formato, lo imprimimos directamente
            default:
                str[a++] = format[i];
                i++;
                break;
        }
    }

    str[a] = '\0';

    va_end(args);
}
