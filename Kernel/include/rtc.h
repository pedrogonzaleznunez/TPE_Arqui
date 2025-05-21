#ifndef RTC_H
#define RTC_H

// hace la interrupción. Según el descriptor
// devuelve un campo del tiempo
int getTimeFragment(char descriptor);

// imprime el tiempo
void printTime();

#endif
