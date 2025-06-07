#include <font.h>
#include <lib.h>
#include <videoDriver.h>

#define LEFT_MARGIN   4
#define TOP_MARGIN    8
#define RIGHT_MARGIN  4
#define BOTTOM_MARGIN 8

static uint64_t currentX = LEFT_MARGIN;
static uint64_t currentY = TOP_MARGIN;

#define BPP   ((VBE_mode_info->bpp) / 8)
#define PITCH ((VBE_mode_info)->pitch)

// Funciones de uso interno en videoDriver.c
void moveLine(int source, int target);
void scrollUp();
void checkScreenLimits();
void checkEndOfHeight();
void checkEndOfLine();
int isOutOfBounds(uint64_t x, uint64_t y);
void moveCurrentX();
int checkSpecialCharacter(char c);

struct vbe_mode_info_structure {
    uint16_t attributes; // deprecated, only bit 7 should be of interest to you, and it
                         // indicates the mode supports a linear frame buffer.
    uint8_t window_a;    // deprecated
    uint8_t window_b;    // deprecated
    uint16_t granularity;// deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;// deprecated; used to switch banks from protected mode
                          // without returning to real mode
    uint16_t pitch;       // number of bytes per horizontal line
    uint16_t width;       // width in pixels
    uint16_t height;      // height in pixels
    uint8_t w_char;       // unused...
    uint8_t y_char;       // ...
    uint8_t planes;
    uint8_t bpp;  // bits per pixel in this mode
    uint8_t banks;// deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;// deprecated; size of a bank, almost always 64 KB but may
                      // be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;// physical address of the linear frame buffer; write
                         // here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;// size of memory in the framebuffer but not
                                 // being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));

typedef struct vbe_mode_info_structure *VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    if (isOutOfBounds(x, y)) return;

    uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * BPP) + (y * PITCH);

    framebuffer[offset] = (hexColor) & 0xFF;
    framebuffer[offset + 1] = (hexColor >> 8) & 0xFF;
    framebuffer[offset + 2] = (hexColor >> 16) & 0xFF;

    return;
}

void putCharInPos(char c, uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t *charBitmap = getFontChar(c);

    if (charBitmap == NULL) return;

    uint8_t charHeight = getFontHeight();// Generalmente 16
    uint8_t charWidth = getFontWidth();  // Generalmente 8

    for (uint8_t auxY = 0; auxY < charHeight; auxY++) {
        for (uint8_t auxX = 0; auxX < charWidth; auxX++) {
            if ((charBitmap[auxY] >> (charWidth - 1 - auxX)) & 0x01) {
                putPixel(hexColor, x + auxX, y + auxY);
            }
        }
    }

    return;
}

void putStringInPos(char *str, uint32_t hexColor, uint64_t x, uint64_t y) {
    if (str == NULL) { return; }

    // La parte que no está en el rango de la pantalla no escribe en memoria
    while (*str) {
        if (!checkSpecialCharacter(*str)) {
            putCharInPos(*str, hexColor, x, y);
            x += getFontWidth();
        }
        str++;
    }

    return;
}

void newLine() {
    currentY += getFontHeight();
    checkEndOfHeight();
    currentX = LEFT_MARGIN;
    return;
}

void checkScreenLimits() {
    checkEndOfLine();
    checkEndOfHeight();
    return;
}

void putChar(char c, uint32_t hexColor) {
    checkScreenLimits();
    if (!checkSpecialCharacter(c)) {
        putCharInPos(c, hexColor, currentX, currentY);
        moveCurrentX();
    }

    return;
}

void checkEndOfLine() {
    // Acá agregado getFontWidth()
    if (currentX + RIGHT_MARGIN + getFontWidth() > VBE_mode_info->width) { newLine(); }
    return;
}

void putString(char *str, uint32_t hexColor) {
    if (str == NULL) { return; }

    while (*str) {
        putChar(*str, hexColor);
        str++;
    }
    return;
}

void moveCurrentX() {
    currentX += getFontWidth();
    checkEndOfLine();
    return;
}

void checkEndOfHeight() {
    // Acá agregado getFontHeight()
    if (currentY + BOTTOM_MARGIN + getFontHeight() > VBE_mode_info->height) {
        scrollUp();
    }
    return;
}

void scrollUp() {
    uint8_t charHeight = getFontHeight();
    uint32_t lineCount =
        (VBE_mode_info->height - TOP_MARGIN - BOTTOM_MARGIN) / charHeight;

    for (int i = 0; i < lineCount - 1; i++) { moveLine(i + 1, i); }

    currentX = LEFT_MARGIN;
    currentY = TOP_MARGIN + (lineCount - 1) * charHeight;

    uint64_t width = VBE_mode_info->width - LEFT_MARGIN - RIGHT_MARGIN;
    uint64_t bytesPerLine = width * BPP;
    uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;

    // limpia la última línea antes de seguir.
    for (int j = 0; j < charHeight; j++) {
        uint8_t *lineStart = framebuffer + LEFT_MARGIN * BPP + (currentY + j) * PITCH;
        memset(lineStart, DEFAULT_BGR_COLOR, bytesPerLine);
    }

    return;
}

void moveLine(int source, int target) {
    uint8_t charHeight = getFontHeight();

    if (source < 0 || source > VBE_mode_info->height / charHeight || target < 0 ||
        target > VBE_mode_info->height / charHeight) {
        // Es un movimiento no válido
        return;
    }
    uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;

    uint64_t width = VBE_mode_info->width - LEFT_MARGIN - RIGHT_MARGIN;
    uint64_t bytesPerLine = width * BPP;
    uint64_t sourceY = source * charHeight + TOP_MARGIN;
    uint64_t targetY = target * charHeight + TOP_MARGIN;

    for (uint32_t j = 0; j < charHeight; j++) {
        uint8_t *sourcePtr = framebuffer + LEFT_MARGIN * BPP + (sourceY + j) * PITCH;
        uint8_t *targetPtr = framebuffer + LEFT_MARGIN * BPP + (targetY + j) * PITCH;

        memcpy(targetPtr, sourcePtr, bytesPerLine);
    }
    return;
}

int isOutOfBounds(uint64_t x, uint64_t y) {
    return !(x < VBE_mode_info->width && y < VBE_mode_info->height);
}

void clearScreen() {
    uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;
    uint32_t total_bytes = VBE_mode_info->width * PITCH * BPP;

    // Usar memset es más eficiente que pixel por pixel
    memset(framebuffer, 0x00, total_bytes);

    currentX = LEFT_MARGIN;
    currentY = TOP_MARGIN;
}

void deleteChar() {
    uint8_t charHeight = getFontHeight();
    uint8_t charWidth = getFontWidth();

    if (currentX - LEFT_MARGIN <= 0) {
        if (currentY > TOP_MARGIN) {
            currentY -= charHeight;
            currentX = VBE_mode_info->width - charWidth - RIGHT_MARGIN;

            uint64_t charsPerLine =
                (VBE_mode_info->width - LEFT_MARGIN - RIGHT_MARGIN) / charWidth;
            currentX = LEFT_MARGIN + (charsPerLine - 1) * charWidth;
        }
        // Si primera columna y primera línea, nada.
    } else {
        currentX -= charWidth;
    }

    for (int i = 0; i < charWidth; i++) {
        for (int j = 0; j < charHeight; j++) {
            putPixel(DEFAULT_BGR_COLOR, currentX + i, currentY + j);
        }
    }
}

int checkSpecialCharacter(char c) {
    switch (c) {
        case '\n':
            newLine();
            break;
        case '\t':
            putString("    ", DEFAULT_CHAR_COLOR);
            break;
        case '\b':
            deleteChar();
            break;
        default:
            return 0;
            break;
    }

    return 1;
}

void drawCircle(uint64_t pos_x, uint64_t pos_y, uint64_t radius, uint32_t hexColor) {
    if (radius == 0) return;

    // Calculamos el rectángulo de exploración acotado a la pantalla
    uint64_t min_x = pos_x > radius ? pos_x - radius : 0;
    uint64_t max_x = (pos_x + radius < VBE_mode_info->width) ? pos_x + radius
                                                             : VBE_mode_info->width - 1;
    uint64_t min_y = pos_y > radius ? pos_y - radius : 0;
    uint64_t max_y = (pos_y + radius < VBE_mode_info->height) ? pos_y + radius
                                                              : VBE_mode_info->height - 1;

    for (uint64_t x = min_x; x <= max_x; x++) {
        for (uint64_t y = min_y; y <= max_y; y++) {
            int64_t dx = (int64_t) x - (int64_t) pos_x;
            int64_t dy = (int64_t) y - (int64_t) pos_y;
            if (dx * dx + dy * dy <= (int64_t) radius * radius) {
                putPixel(hexColor, x, y);
            }
        }
    }
    return;
}

void drawRec(uint64_t from_x, uint64_t from_y, uint64_t to_x, uint64_t to_y,
             uint32_t hexColor) {
    if (from_x > to_x || from_y > to_y) return;

    // Clamp de los límites para no salirnos de la pantalla
    uint64_t start_x = from_x < VBE_mode_info->width ? from_x : VBE_mode_info->width;
    uint64_t end_x = to_x < VBE_mode_info->width ? to_x : VBE_mode_info->width - 1;
    uint64_t start_y = from_y < VBE_mode_info->height ? from_y : VBE_mode_info->height;
    uint64_t end_y = to_y < VBE_mode_info->height ? to_y : VBE_mode_info->height - 1;

    for (uint64_t x = start_x; x <= end_x; x++) {
        for (uint64_t y = start_y; y <= end_y; y++) { putPixel(hexColor, x, y); }
    }
    return;
}

void fillScreen(uint32_t hexColor) {
    uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;
    uint32_t total_pixels = VBE_mode_info->width * VBE_mode_info->height;

    if (hexColor) {
        uint8_t r = (hexColor >> 16) & 0xFF;
        uint8_t g = (hexColor >> 8) & 0xFF;
        uint8_t b = hexColor & 0xFF;

        for (uint32_t i = 0; i < total_pixels; i++) {
            uint32_t offset = i * 3;
            framebuffer[offset] = b;
            framebuffer[offset + 1] = g;
            framebuffer[offset + 2] = r;
        }
    } else {
        clearScreen();
    }

    currentX = LEFT_MARGIN;
    currentY = TOP_MARGIN;
}
