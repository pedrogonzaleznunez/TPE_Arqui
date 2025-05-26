#include <keyboard.h>
#include <stdint.h>
#include <time.h>

#include <videoDriver.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
  switch (irq) {
  case 0:
    int_20();
    break;
  case 1:
    int_21();
    break;
  }

  return;
}

static void int_20() { timer_handler(); }

static void int_21() { keyboardHandler(); }
