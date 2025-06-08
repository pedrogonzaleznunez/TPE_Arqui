#include <keyboard.h>
#include <naiveConsole.h>
#include <registers.h>
#include <rtc.h>
#include <sounds.h>
#include <time.h>
#include <videoDriver.h>

#include <interrupts.h>
#include <syscalls.h>

#include <stdarg.h>
#include <stddef.h>// para el NULL
#include <stdint.h>

// args variables:
// https://www.programacionenc.net/index.php?option=com_content&view=article&id=61:funciones-en-c-con-lista-de-argumentos-variable&catid=37:programacion-cc&Itemid=55

int64_t syscallDispatcher(uint64_t syscallId, ...) {
    va_list arguments;             // lista de argumentos
    va_start(arguments, syscallId);// ultimo argumento que no es variable

    // syscallId: numero de syscall
    switch (syscallId) {

        //write
        case 0:
            int64_t fd_write = va_arg(arguments, int64_t);
            const char *buf_write = va_arg(arguments, const char *);
            int64_t count_write = va_arg(arguments, int64_t);
            return write(fd_write, buf_write, count_write);

        //read
        case 1:
            int64_t fd_read = va_arg(arguments, int64_t);
            char *buf_read = va_arg(arguments, char *);
            int64_t count_read = va_arg(arguments, int64_t);
            return read(fd_read, buf_read, count_read);

        // get regs
        case 2:
            register_set_t *regs = va_arg(arguments, register_set_t *);
            return setRegisters(regs);

        // get time
        case 3:
            time_t *time = va_arg(arguments, time_t *);
            setTime(time);
            return 1;

        // delete char
        case 4:
            deleteChar();
            return 1;

        // start beep
        case 20:
            uint32_t frecuency_start_beep = va_arg(arguments, uint32_t);
            playSound(frecuency_start_beep);
            return 1;

        // stop beep
        case 21:
            noSound();
            return 1;

        // beep
        case 22:
            uint32_t frecuency_beep = va_arg(arguments, uint32_t);
            int64_t ticks_beep = va_arg(arguments, int64_t);
            _sti();
            playSound(frecuency_beep);
            timerWait(ticks_beep);
            noSound();
            return 1;

        // sleep
        case 23:
            int64_t ticks_sleep = va_arg(arguments, int64_t);
            _sti();
            timerWait(ticks_sleep);
            return 1;

        // clear screen
        case 30:
            clearScreen();
            return 1;

        // draw circle
        case 31:
            uint64_t pos_x_circle = va_arg(arguments, uint64_t);
            uint64_t pos_y_circle = va_arg(arguments, uint64_t);
            uint64_t radius_circle = va_arg(arguments, uint64_t);
            uint32_t hexColor_circle = va_arg(arguments, uint32_t);
            drawCircle(pos_x_circle, pos_y_circle, radius_circle, hexColor_circle);
            return 1;

        // draw rectangle
        case 32:
            uint64_t from_x_rec = va_arg(arguments, uint64_t);
            uint64_t from_y_rec = va_arg(arguments, uint64_t);
            uint64_t to_x_rec = va_arg(arguments, uint64_t);
            uint64_t to_y_rec = va_arg(arguments, uint64_t);
            uint32_t hexColor_rec = va_arg(arguments, uint32_t);
            drawRec(from_x_rec, from_y_rec, to_x_rec, to_y_rec, hexColor_rec);
            return 1;

        // fill screen
        case 33:
            uint32_t hexColor_fill_screen = va_arg(arguments, uint32_t);
            fillScreen(hexColor_fill_screen);
            return 0;

        // draw pixel
        case 34:
            uint64_t pos_x_pixel = va_arg(arguments, uint64_t);
            uint64_t pos_y_pixel = va_arg(arguments, uint64_t);
            uint32_t hexColor_pixel = va_arg(arguments, uint32_t);
            putPixel(hexColor_pixel, pos_x_pixel, pos_y_pixel);
            return 1;
    }

    va_end(arguments);

    return -1;
}

int64_t write(int64_t fd, const char *buf, int64_t count) {
    // handler de la syscall de write video

    int format = (fd == 1 ? DEFAULT_CHAR_COLOR : ERROR_CHAR_COLOR);

    for (int64_t i = 0; i < count; i++) { putChar(buf[i], format); }

    return 0;
}

int64_t read(int64_t fd, char *buf, int64_t count) {
    // handler de la syscall de lectura
    if (buf == NULL || count < 0) { return -1; }

    _sti();

    int64_t bytesRead = 0;

    while (bytesRead < count) {
        int8_t c = bufferRead();
        if (c != -1) {
            if (c == 0) { break; }
            buf[bytesRead++] = c;
        }
    }

    return bytesRead;
}
