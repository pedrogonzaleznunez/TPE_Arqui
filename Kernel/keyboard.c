#include <keyboard.h>
#include <naiveConsole.h>
#include <videoDriver.h>

// tengo una función getKey que me devuelve un hexa.
// Como se guarda varias veces la misma, me fijo que la nueva
// sea distinta de la anterior le hago un ncPrintHex(tecla)

void printKey(char key) {
  // valores sacados de Scan Code Set 1 en https://wiki.osdev.org/PS/2_Keyboard
  int printable[] = {
      [0x01] = -1, // escape no tiene carácter imprimible
      [0x02] = '1',  [0x03] = '2', [0x04] = '3',  [0x05] = '4',
      [0x06] = '5',  [0x07] = '6', [0x08] = '7',  [0x09] = '8',
      [0x0A] = '9',  [0x0B] = '0', [0x0C] = '-',  [0x0D] = '=',
      [0x0E] = -1, // backspace no es imprimible
      [0x0F] = -1, // tab no es imprimible
      [0x10] = 'q',  [0x11] = 'w', [0x12] = 'e',  [0x13] = 'r',
      [0x14] = 't',  [0x15] = 'y', [0x16] = 'u',  [0x17] = 'i',
      [0x18] = 'o',  [0x19] = 'p', [0x1A] = '[',  [0x1B] = ']',
      [0x1C] = -1, // enter no es imprimible
      [0x1D] = -1, // left control no es imprimible
      [0x1E] = 'a',  [0x1F] = 's', [0x20] = 'd',  [0x21] = 'f',
      [0x22] = 'g',  [0x23] = 'h', [0x24] = 'j',  [0x25] = 'k',
      [0x26] = 'l',  [0x27] = ';', [0x28] = '\'', [0x29] = '`',
      [0x2A] = -1, // left shift no es imprimible
      [0x2B] = '\\', [0x2C] = 'z', [0x2D] = 'x',  [0x2E] = 'c',
      [0x2F] = 'v',  [0x30] = 'b', [0x31] = 'n',  [0x32] = 'm',
      [0x33] = ',',  [0x34] = '.', [0x35] = '/',
      [0x36] = -1,  // right shift no es imprimible
      [0x37] = '*', // (keypad) * es imprimible
      [0x38] = -1,  // left alt no es imprimible
      [0x39] = ' ', // Espacio
      [0x3A] = -1,  // CapsLock no es imprimible
      [0x3B] = -1,  // F1 no es imprimible
      [0x3C] = -1,  // F2 no es imprimible
      [0x53] = '.', // imprimible
  };

  if (key > 0 && key < 0x53 && printable[key] != -1) {
    ncPrintChar(printable[key]);
    putChar(printable[key], 0xFFFFFF);
  } else {
    switch (key) {
    case 0x0E:
      ncDeleteChar(1);
      break;
    case 0x0F:
      ncPrintChar(' ');
      ncPrintChar(' ');
      ncPrintChar(' ');
      ncPrintChar(' ');
      putString("    ", 0xFFFFFF);
      break;
    case 0x1C:
      ncNewline();
      newLine();
      break;
    default:
      break;
    }
  }
}

void printPressedKey() {
  char aux, key;
  key = getKey();
  aux = key;
  if (!(aux >> 7)) {
    printKey(key);
  }
}
