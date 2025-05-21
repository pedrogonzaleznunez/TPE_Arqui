#include <time.h>
#include <stdint.h>
#include <keyboard.h>

#include <naiveConsole.h>

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

void int_20() {
	timer_handler();
}

void int_21() {
  // en este caso el handler es imprimir la tecla
  printPressedKey();
//   ncClear();
//   ncPrint("Tecla presionada ");
}
