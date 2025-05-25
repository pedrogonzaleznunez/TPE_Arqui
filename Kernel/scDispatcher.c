#include <stdint.h>
#include <stdarg.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboard.h>

static int32_t sys_write(int64_t fd, const char * buf, int64_t count); // write video
static int32_t sys_read(int64_t fd, char * buf, int64_t count); //read

// las funciones devuelven un int, deberia devolver un int? 
// syscalls del kernel
// https://www.programacionenc.net/index.php?option=com_content&view=article&id=61:funciones-en-c-con-lista-de-argumentos-variable&catid=37:programacion-cc&Itemid=55

void syscallDispatcher(uint64_t syscallId, ...) {
    va_list arguments; //lista de argumentos
    va_start(arguments, syscallId);  //ultimo argumento que no es variable

    // syscallId es el numero de syscall
  	switch (syscallId) {

        case 0:
            int64_t fd = va_arg(arguments, int64_t);
    		const char * buf = va_arg(arguments, const char *);
            int64_t count = va_arg(arguments, int64_t);
            sys_write(fd, buf, count); 
            break;
        case 1:
            int64_t fd1 = va_arg(arguments, int64_t);
    		char * buf1 = va_arg(arguments, char *);
            int64_t count1 = va_arg(arguments, int64_t);
            sys_read(fd1, buf1, count1);
            break;
	}
  	
    va_end(arguments);

	return;
}

// int32_t sys_write(int64_t fd, const void * buf, int64_t count);
// int32_t sys_read(int64_t fd, void * buf, int64_t count);
// int32_t sys_start_beep(uint32_t frecuence);
// int32_t sys_stop_beep(void);

int32_t sys_write(int64_t fd, const char * buf, int64_t count) {
  // handler de la syscall de write video
    int format = 0x0F;  // provisiorio como default
    
    if(fd){
        format = 0x0C; // por enunciado la salida de error es escribir en pantalla con color rojo
    }

    for(int64_t i = 0; i < count; i++){
        putChar(buf[i], format);
    }
    
    return 0;
}

int32_t sys_read(int64_t fd, char * buf, int64_t count) {
  // handler de la syscall de lectura
    
    int64_t i = 0;
    char key = 0;

    while(i < count && key != 0x1C){
        char aux;
        key = getKey();
        aux = key;
        if (!(aux >> 7)) { // el bit mas significativo debe ser cero
            char c = getChar(key);
            buf[i] = c;
            putPixel(0x00FF00, 100 + i, 100 + i); // color verde
            putChar(c, 0x00FF00);
            i++;
        }
    }
    putString("Fin de lectura", 0xFF00FF);
    return 0;
}
