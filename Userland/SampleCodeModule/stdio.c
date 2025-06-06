#include "include/stdio.h"
#include "include/syscalls.h"
#include <stdarg.h>
#include <stdint.h>


// todo manejar colores ??

static char buffer[64] = {0};

static void printBase(int fd, int num, int base);
void vfprintf(int fd, const char *format, va_list args);
//static 
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

void puts(const char *str) {
    while (*str) { putchar(*str++); }
    putchar('\n');
}

void putchar(const char c) {
    // sys_write(FD_STDOUT, &c, 1);
    sys_write(FD_STDOUT, &c, 1);// syscall para escribir un caracter
}

int getchar() {
    char c;
    // sys_read(FD_STDIN, &c, 1);
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
                        // sys_write(fd, &c, 1);
                        sys_write(fd, &c, 1);// syscall para escribir un caracter
                        break;
                    }
                    case 's':
                        fprintf(fd, va_arg(args, char *));
                        break;
                    case '%':
                        sys_write(fd, "%", 1);// syscall para escribir un caracter
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
    int i = 0;
    int args_read = 0;
    char c;
    while (format[i] != 0) {
        switch (format[i]) {
            case '%':
                i++;
                switch (format[i]) {
                    case 'd':
                        int64_t num = 0;
                        uint8_t negative = 0;

                        c = getchar();

                        if (c != '-' && (c < '0' || c > '9')) {
                            while ((c = getchar()) !=
                                   '\n');// empty input buffer
                            break;
                        };

                        do {
                            if (c == '-') negative = 1;
                            else
                                num = num * 10 + c - '0';
                        } while (((c = getchar()) >= '0' && c <= '9') ||
                                 (num == 0 && c == '-'));

                        *va_arg(args, int *) = num * (negative ? -1 : 1);
                        args_read++;
                        break;
                    case 'c':
                        *va_arg(args, char *) = getchar();
                        args_read++;
                        break;
                    case 's':
                        char *str = va_arg(args, char *);
                        while ((c = getchar()) != ' ' && c != '\n') {
                            *str = c;
                            str++;
                        }
                        *str = 0;
                        args_read++;
                        break;
                }
        }
        i++;
    }
    return args_read;
}

int vsscanf(const char *buffer, const char *format, va_list args) {
    int form_i = 0;
    int buf_i = 0;
    int args_read = 0;
    while (format[form_i] != 0) {
        switch (format[form_i]) {
            case '%':
                form_i++;
                switch (format[form_i]) {
                    case 'd': {
                        int num = 0;
                        uint8_t read_num = 0, negative = 0;

                        if (buffer[buf_i] != '-' &&
                            (buffer[buf_i] < '0' || buffer[buf_i] > '9'))
                            break;

                        if (buffer[buf_i] == '-') {
                            negative = 1;
                            buf_i++;
                        };

                        while (buffer[buf_i] >= '0' && buffer[buf_i] <= '9') {
                            num = num * 10 + buffer[buf_i] - '0';
                            buf_i++;
                            read_num = 1;
                        }

                        *va_arg(args, int *) = num * (negative ? -1 : 1);
                        args_read += read_num;
                        break;
                    }
                    case 'c': {
                        *va_arg(args, char *) = buffer[buf_i++];
                        args_read++;
                        break;
                    }
                    case 's': {
                        char *str = va_arg(args, char *);
                        while (buffer[buf_i] != ' ' && buffer[buf_i] != '\n' &&
                               buffer[buf_i] != 0) {
                            *(str++) = buffer[buf_i];
                            buf_i++;
                        }
                        *str = 0;
                        args_read++;
                        break;
                    }
                    default:
                        break;
                }
        }
        form_i++;
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

