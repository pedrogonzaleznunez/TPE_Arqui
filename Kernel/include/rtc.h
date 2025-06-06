#include <stdint.h>

#ifndef RTC_H
#define RTC_H


// Retrieved from https://stanislavs.org/helppc/cmos_ram.html 
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define YEAR 0x09
#define MONTH 0x08
#define DAY 0x07


// hace la interrupción. Según el descriptor
// devuelve un campo del tiempo
int getTimeFragment(char descriptor);

int64_t get_seconds();
int64_t get_minutes();
int64_t get_hours();
int64_t get_day();
int64_t get_month();
int64_t get_year();

// imprime el tiempo
void printTime(); 

unsigned int rtcTypeToDecimal(unsigned char time);


#endif
