#include <keyboard.h>

#define BUFFER_DEFAULT_SIZE 1024
#define MAX_SCAN_CODE       0x53

static uint8_t buffer[BUFFER_DEFAULT_SIZE] = {0};
static uint8_t keysState[MAX_SCAN_CODE + 1] = {0};
static uint32_t writeIdx = 0;
static uint32_t readIdx = 0;

static uint32_t shift_pressed = 0;
static uint32_t caps_lock_on = 0;

extern uint8_t toSave_registers_flag;

// valores sacados de Scan Code Set 1 en https://wiki.osdev.org/PS/2_Keyboard
// Se podría agregar que el driver de video haga la parte de impresión de
// caracteres especiales En printable guardemos '\n', '\t'...
static char printable[] = {
    // podemos sacar las entradas de -1 y automaticamente se ponen en cero (y
    // cambiar el checkeo)
    [0x01] = 27,// escape ASCII 27 \e
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = '\b',// backspace (8)
    [0x0F] = '\t',// tab
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n',// enter
    [0x1D] = 0,   // left control no es imprimible
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    [0x2A] = -1,// left shift no es imprimible
    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x36] = -1, // right shift no es imprimible
    [0x37] = '*',// (keypad) * es imprimible
    [0x38] = -1, // left alt no es imprimible
    [0x39] = ' ',// Espacio
    [0x3A] = -1, // CapsLock no es imprimible
    [0x3B] = -1, // F1 no es imprimible
    [0x3C] = -1, // F2 no es imprimible
    [0x3D] = -1, // F3 no es imprimible
    [0x3E] = -1, // F4 no es imprimible
    [0x3F] = -1, // F5 no es imprimible
    [0x40] = -1, // F6 no es imprimible
    [0x41] = -1, // F7 no es imprimible
    [0x42] = -1, // F8 no es imprimible
    [0x43] = -1, // F9 no es imprimible
    [0x44] = -1, // F10 no es imprimible
    [0x48] = ARROW_UP_CODE,
    [0x4B] = ARROW_LEFT_CODE,
    [0x4D] = ARROW_RIGHT_CODE,
    [0x50] = ARROW_DOWN_CODE,
    [0x53] = '.',
};

static char printable_shift[] = {
    [0x01] = 27,// escape no tiene carácter imprimible
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$', [0x06] = '%', [0x07] = '^',
    [0x08] = '&', [0x09] = '*', [0x0A] = '(', [0x0B] = ')', [0x0C] = '_', [0x0D] = '+',
    [0x27] = ':',// ; pero con Shift
    [0x28] = '"',// '''
    [0x29] = '~',// '`'
    [0x2B] = '|',// '\'
    [0x33] = '<',// ','
    [0x34] = '>',// '.'
    [0x35] = '?',// '/'
};

char getChar(uint8_t scanCode) {
    if (scanCode > 0 && scanCode < MAX_SCAN_CODE) {
        char aux = printable[scanCode];

        if ((caps_lock_on) && (aux >= 'a' && aux <= 'z')) {
            return aux + ('A' - 'a');// Convertir a mayúscula
        }

        if ((shift_pressed) && (aux >= 'a' && aux <= 'z')) {
            return aux + ('A' - 'a');// Convertir a mayúscula
        }

        if (shift_pressed && scanCode <= 0x0D) { return printable_shift[scanCode]; }
        return aux;
    } else {
        return 0;
    }
}

void keyboardHandler() {
    uint8_t key = getKey();// debe ser unsigned para detectar el release
    uint8_t aux = key;

    if (key == SHIFT_RELEASED_LEFT ||
        key == SHIFT_RELEASED_RIGHT) {// L or R Shift released
        shift_pressed = 0;
        return;
    }

    if (key == 0x2A || key == 0x36) {// L or R Shift pressed
        shift_pressed = 1;
        return;
    }

    if (key == 0x3A) {
        // Caps Lock (el release de caps lock no lo manejo, solo me importa cuando se apreta)
        caps_lock_on = !caps_lock_on;
        return;
    }

    if (key == F1_REGISTER_BACKUP) { toSave_registers_flag = 1; }

    // si no es un release, que lo guarde en el buffer
    if (!(aux >> 7)) {
        keysState[key] = 1;
        char c = getChar(key);
        if (!isBufferFull()) {
            buffer[writeIdx++] = c;
            writeIdx %= BUFFER_DEFAULT_SIZE;
        }
    } else {
        keysState[key & 0x7F] = 0;
    }
}

char bufferRead() {
    if (!isBufferEmpty()) {
        char toRet = buffer[readIdx++];
        readIdx %= BUFFER_DEFAULT_SIZE;
        return toRet;
    }

    return -1;
}

int isBufferEmpty() {
    return writeIdx == readIdx;
}

int isBufferFull() {
    return (writeIdx + 1) % BUFFER_DEFAULT_SIZE == readIdx;
}

int8_t getKeyState(uint8_t scancode) {
    if (scancode > 0 && scancode <= MAX_SCAN_CODE) { return keysState[scancode]; }
    return -1;
}
