#include <rtc.h>

// TODO HOMOGENEIZAR LOS TIPOS DE DATOS QUE USA

int64_t getSeconds() {
    return rtcTypeToDecimal(getTimeFragment(SECONDS));
}

int64_t getMinutes() {
    return rtcTypeToDecimal(getTimeFragment(MINUTES));
}

int64_t getHours() {
    return rtcTypeToDecimal(getTimeFragment(HOURS));
}

int64_t getDay() {
    return rtcTypeToDecimal(getTimeFragment(DAY));
}

int64_t getMonth() {
    return rtcTypeToDecimal(getTimeFragment(MONTH));
}

int64_t getYear() {
    return rtcTypeToDecimal(getTimeFragment(YEAR));
}


// https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
unsigned int rtcTypeToDecimal(unsigned char time) {
    return (unsigned int) ((time >> 4) * 10 + (time & 0x0F));
}

void setTime(time_t *time) {
    time->seconds = getSeconds();
    time->minutes = getMinutes();
    time->hours = getHours();
    time->day = getDay();
    time->month = getMonth();
    time->year = getYear();
}
