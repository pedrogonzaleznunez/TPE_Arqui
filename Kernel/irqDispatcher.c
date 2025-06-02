#include <keyboard.h>
#include <stdint.h>
#include <time.h>

#include <videoDriver.h>

static void int_20();
static void int_21();

static void (*functions[])() = {
    &int_20,// IRQ 0
    &int_21 // IRQ 1
};

void irqDispatcher(uint64_t irq) {
  functions[irq]();
  return;

}

static void int_20() {
    timer_handler();
}

static void int_21() { 
  keyboardHandler(); 
 }

