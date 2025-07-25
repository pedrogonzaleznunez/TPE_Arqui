#include <naiveConsole.h>

// Funciones agregadas internas
void ncMoveLine(int target, int source);
void ncScrollUp();
void ncCheckEndOfScreen();

static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *) 0xB8000;
static uint8_t *currentVideo = (uint8_t *) 0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void ncPrint(const char *string) {
    int i;

    for (i = 0; string[i] != 0; i++) { ncPrintChar(string[i]); }
}

void ncPrintChar(char character) {
    ncPrintCharColor(character, 0x0F);
}

void ncNewline() {
    do { ncPrintChar(' '); } while ((uint64_t) (currentVideo - video) % (width * 2) != 0);
    ncCheckEndOfScreen();
}

void ncPrintDec(uint64_t value) {
    ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value) {
    ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value) {
    ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear() {
    int i;

    for (i = 0; i < height * width; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x00;
    }

    currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    // Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

void ncPrintColor(const char *string, const int format) {
    int i;

    for (i = 0; string[i] != 0; i++) { ncPrintCharColor(string[i], format); }
}

void ncPrintCharColor(char character, const int format) {
    ncCheckEndOfScreen();
    *currentVideo++ = character;
    *currentVideo++ = format;
}

void ncScrollUp() {
    for (int i = 0; i < height - 1; i++) {
        ncMoveLine(i, i + 1);
    }// limpia la última línea antes de seguir.
    for (int i = 0; i < width; i++) {
        int idx = ((height - 1) * width + i) * 2;
        video[idx] = ' ';
        video[idx + 1] = 0x0F;
    }
    currentVideo -= width * 2;
}

void ncMoveLine(int target, int source) {
    for (int i = 0; i < width; i++) {
        int srcIdx = (source * width + i) * 2;
        int tgtIdx = (target * width + i) * 2;
        video[tgtIdx] = video[srcIdx];
        video[tgtIdx + 1] = video[srcIdx + 1];
    }
}

void ncDeleteChar(char amount) {
    for (int i = 0; i < amount; i++) {
        *currentVideo-- = ' ';
        *currentVideo-- = 0x00;
    }
}

void ncCheckEndOfScreen() {
    if (currentVideo == video + width * height * 2) { ncScrollUp(); }
}
