#include <stdint.h>

#ifndef RTC_H
#define RTC_H

// Retrieved from https://stanislavs.org/helppc/cmos_ram.html
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS   0x04
#define YEAR    0x09
#define MONTH   0x08
#define DAY     0x07

typedef struct time {
    uint64_t seconds;
    uint64_t minutes;
    uint64_t hours;
    uint64_t day;
    uint64_t month;
    uint64_t year;
} time_t;

// hace la interrupción. Devuelve un campo del tiempo según el descriptor.
int getTimeFragment(char descriptor);

int64_t getSeconds();
int64_t getMinutes();
int64_t getHours();
int64_t getDay();
int64_t getMonth();
int64_t getYear();

unsigned int rtcTypeToDecimal(unsigned char time);

void setTime(time_t *time);

#endif
