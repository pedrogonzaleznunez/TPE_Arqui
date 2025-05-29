#include <keyboard.h>
#include <naiveConsole.h>
#include <stdarg.h>
#include <stdint.h>
#include <videoDriver.h>

#include <interrupts.h>

#include <stddef.h> // para el NULL

static int64_t sys_write(int64_t fd, const char *buf,
                         int64_t count);                       // write video
static int64_t sys_read(int64_t fd, char *buf, int64_t count); // read

// las funciones devuelven un int, deberia devolver un int?
// syscalls del kernel
// https://www.programacionenc.net/index.php?option=com_content&view=article&id=61:funciones-en-c-con-lista-de-argumentos-variable&catid=37:programacion-cc&Itemid=55

int64_t syscallDispatcher(uint64_t syscallId, ...) {
  va_list arguments;              // lista de argumentos
  va_start(arguments, syscallId); // ultimo argumento que no es variable

  // syscallId es el numero de syscall
  switch (syscallId) {

  case 0:
    int64_t fd = va_arg(arguments, int64_t);
    const char *buf = va_arg(arguments, const char *);
    int64_t count = va_arg(arguments, int64_t);
    return sys_write(fd, buf, count);
    break;
  case 1:
    int64_t fd1 = va_arg(arguments, int64_t);
    char *buf1 = va_arg(arguments, char *);
    int64_t count1 = va_arg(arguments, int64_t);
    return sys_read(fd1, buf1, count1);
    break;
  }

  va_end(arguments);

  return -1;
}

// int32_t sys_write(int64_t fd, const void * buf, int64_t count);
// int32_t sys_read(int64_t fd, void * buf, int64_t count);
// int32_t sys_start_beep(uint32_t frecuence);
// int32_t sys_stop_beep(void);

int64_t sys_write(int64_t fd, const char *buf, int64_t count) {
  // handler de la syscall de write video
  int format = fd ? ERROR_CHAR_COLOR : DEFAULT_CHAR_COLOR;

  for (int64_t i = 0; i < count; i++) {
    putChar(buf[i], format);
  }

  return 0;
}

int64_t sys_read(int64_t fd, char *buf, int64_t count) {
  // handler de la syscall de lectura
  if (buf == NULL || count < 0) {
    return -1;
  }

  // Por alguna razón, no estaban habilitadas las interrupciones al entrar a la
  // syscall
  _sti();

  int64_t bytesRead = 0;

  while (bytesRead < count) {
    char c = bufferRead();
    if (c != -1) {
      if (c == '\n' || c == 0) {
        break;
      }
      buf[bytesRead++] = c;
    }
  }

  return bytesRead;
}
