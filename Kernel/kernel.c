#include <font.h>
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include <videoDriver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void *initializeKernelBinary() {

  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);

  clearBSS(&bss, &endOfKernel - &bss);

  return getStackBase();
}

void printSystemTime() {
  // uint64_t time = getSystemTime();
  // ncPrint("System time: ");
  // ncPrintDec(time);
  // ncNewline();
}

int main() {

  load_idt();
  // Inicio kernel
  // putString("Bienvenido a x64BareBones!", 0xFF00FF);
  // newLine();

  // printSystemTime();

  // ncPrint("[Kernel Main]");
  // ncNewline();
  // ncPrint("  Sample code module at 0x");
  // ncPrintHex((uint64_t)sampleCodeModuleAddress);
  // ncNewline();
  // ncPrint("  Calling the sample code module returned: ");
  putString("Entra al Userland:", 0xFF00FF);
  newLine();

  ((EntryPoint)sampleCodeModuleAddress)();

  newLine();
  newLine();
  putString("Termina el módulo de Userland", 0xFF00FF);
  //  ncNewline();

  // ncPrint("  Sample data module at 0x");
  // ncPrintHex((uint64_t)sampleDataModuleAddress);
  // ncNewline();
  // ncPrint("  Sample data module contents: ");
  // ncPrint((char *)sampleDataModuleAddress);
  // ncNewline();

  // call the main from the

  while (1)
    ;

  return 0;
}
