#include <sounds.h>
#include <time.h>

// https://wiki.osdev.org/PC_Speaker

// Play sound using built-in speaker
void playSound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;

    //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div));
    outb(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) { outb(0x61, tmp | 3); }
}

void noSound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void beep(void) {
    playSound(440);
    timerWait(40);
    noSound();
}
