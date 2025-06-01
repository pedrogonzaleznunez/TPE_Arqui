#include <stdint.h>
#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
    ticks++;
}

int ticks_elapsed() {
    return ticks;
}

int seconds_elapsed() {
    return ticks / 18;
}

void timerWait(uint32_t ticksToWait) {
    long initialTicks = ticks;
    while (ticks - initialTicks < ticksToWait);
}
