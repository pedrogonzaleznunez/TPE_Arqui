#include <keyboard.h>
#include <naiveConsole.h>
#include <registers.h>
#include <sounds.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <videoDriver.h>
#include <rtc.h>

#include <interrupts.h>
#include <syscalls.h>
#include <rtc.h>

#include <stddef.h>// para el NULL

// Pasé las funciones a un .h
// static int64_t sys_write(int64_t fd, const char *buf,
//                          int64_t count);                      // write video
// static int64_t sys_read(int64_t fd, char *buf, int64_t count);// read
// static int64_t sys_start_beep(uint32_t frecuency);
// static int64_t sys_stop_beep(void);
// static int64_t sys_sleep(int64_t ticks);
// static int64_t sys_clear_screen(void);
// static int64_t sys_beep(uint32_t frecuency, int64_t ticks);

// static int64_t sys_print_regs(void);

// static int64_t sys_draw_circle(uint64_t pos_x, uint64_t pos_y, uint64_t radius,
//                                uint32_t hexColor);
// static int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x,
//                             uint64_t to_y, uint32_t hexColor);
// static int64_t sys_fill_screen(uint32_t hexColor);
// static int64_t sys_draw_pixel(uint64_t pos_x, uint64_t pos_y, uint32_t hexColor);


// las funciones devuelven un int, deberia devolver un int?
// syscalls del kernel
// https://www.programacionenc.net/index.php?option=com_content&view=article&id=61:funciones-en-c-con-lista-de-argumentos-variable&catid=37:programacion-cc&Itemid=55

int64_t syscallDispatcher(uint64_t syscallId, ...) {
    va_list arguments;             // lista de argumentos
    va_start(arguments, syscallId);// ultimo argumento que no es variable

    // syscallId es el numero de syscall
    switch (syscallId) {

        //write
        case 0:
            int64_t fd_write = va_arg(arguments, int64_t);
            const char *buf_write = va_arg(arguments, const char *);
            int64_t count_write = va_arg(arguments, int64_t);
            return sys_write(fd_write, buf_write, count_write);

        //read
        case 1:
            int64_t fd_read = va_arg(arguments, int64_t);
            char *buf_read = va_arg(arguments, char *);
            int64_t count_read = va_arg(arguments, int64_t);
            return sys_read(fd_read, buf_read, count_read);

        // get regs
        case 2:
            return sys_print_regs();

        // get time
        case 3:
            time_t * time = va_arg(arguments, time_t *);
            return sys_get_time(time);

        // start beep
        case 20:
            uint32_t frecuency_start_beep = va_arg(arguments, uint32_t);
            return sys_start_beep(frecuency_start_beep);

        // stop beep
        case 21:
            return sys_stop_beep();

        // beep
        case 22:
            uint32_t frecuency_beep = va_arg(arguments, uint32_t);
            int64_t ticks_beep = va_arg(arguments, int64_t);
            return sys_beep(frecuency_beep, ticks_beep);

        // sleep
        case 23:
            int64_t ticks_sleep = va_arg(arguments, int64_t);
            return sys_sleep(ticks_sleep);

        // clear screen
        case 30:
            return sys_clear_screen();

        // clear screen

        // draw circle
        case 31:
            uint64_t pos_x_circle = va_arg(arguments, uint64_t);
            uint64_t pos_y_circle = va_arg(arguments, uint64_t);
            uint64_t radius_circle = va_arg(arguments, uint64_t);
            uint32_t hexColor_circle = va_arg(arguments, uint32_t);
            return sys_draw_circle(pos_x_circle, pos_y_circle, radius_circle,
                                   hexColor_circle);

        // draw rectangle
        case 32:
            uint64_t from_x_rec = va_arg(arguments, uint64_t);
            uint64_t from_y_rec = va_arg(arguments, uint64_t);
            uint64_t to_x_rec = va_arg(arguments, uint64_t);
            uint64_t to_y_rec = va_arg(arguments, uint64_t);
            uint32_t hexColor_rec = va_arg(arguments, uint32_t);
            return sys_draw_rec(from_x_rec, from_y_rec, to_x_rec, to_y_rec, hexColor_rec);

        // fill screen
        case 33:
            uint32_t hexColor_screen = va_arg(arguments, uint32_t);
            return sys_fill_screen(hexColor_screen);

        // draw pixel
        case 34:
            uint64_t pos_x_pixel = va_arg(arguments, uint64_t);
            uint64_t pos_y_pixel = va_arg(arguments, uint64_t);
            uint32_t hexColor_pixel = va_arg(arguments, uint32_t);
            return sys_draw_pixel(pos_x_pixel, pos_y_pixel, hexColor_pixel);
    }

    va_end(arguments);

    return -1;
}


int64_t sys_write(int64_t fd, const char *buf, int64_t count) {
    // handler de la syscall de write video

    int format = (fd == 1 ? DEFAULT_CHAR_COLOR : ERROR_CHAR_COLOR);

    for (int64_t i = 0; i < count; i++) { putChar(buf[i], format); }

    return 0;
}

int64_t sys_read(int64_t fd, char *buf, int64_t count) {
    // handler de la syscall de lectura
    if (buf == NULL || count < 0) { return -1; }

    // Por alguna razón, no estaban habilitadas las interrupciones al entrar a
    // la syscall
    _sti();

    int64_t bytesRead = 0;

    while (bytesRead < count) {
        char c = bufferRead();
        if (c != -1) {
            if (c == '\n' || c == 0) { break; }
            buf[bytesRead++] = c;
        }
    }

    
    return bytesRead;
}

int64_t sys_draw_circle(uint64_t pos_x, uint64_t pos_y, uint64_t radius,
                        uint32_t hexColor) {
    // handler de la syscall de dibujar un círculo
    drawCircle(pos_x, pos_y, radius, hexColor);
    return 0;
}

int64_t sys_draw_rec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y,
                     uint32_t hexColor) {
    // handler de la syscall de dibujar un cuadrado
    drawRec(from_x, from_y, to_x, to_y, hexColor);
    return 0;
}

int64_t sys_draw_pixel(uint64_t pos_x, uint64_t pos_y, uint32_t hexColor) {
    // handler de la syscall de dibujar un pixel
    putPixel(hexColor, pos_x, pos_y);
    return 0;
}

int64_t sys_fill_screen(uint32_t hexColor) {
    // handler de la syscall de llenar la pantalla
    fillScreen(hexColor);
    return 0;
}

int64_t sys_start_beep(uint32_t frecuency) {
    playSound(frecuency);
    return 1;
}

int64_t sys_stop_beep(void) {
    noSound();
    return 1;
}

int64_t sys_sleep(int64_t ticks) {
    timerWait(ticks);
    return 1;
}

int64_t sys_clear_screen(void) {
    clearScreen();
    return 1;
}

int64_t sys_beep(uint32_t frecuency, int64_t ticks) {
    playSound(frecuency);
    timerWait(ticks);
    noSound();
    return 1;
}

int64_t sys_print_regs(void) {
    regs_print(0);
    return 1;
}

int64_t sys_get_time(time_t * time){
    set_time(time);
    return 1;
}

int64_t sys_get_regs(register_set_t * registers){
    set_registers(registers);
    return 1;
}
