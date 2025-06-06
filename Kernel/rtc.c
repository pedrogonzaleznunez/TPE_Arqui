#include <naiveConsole.h>
#include <rtc.h>

// TODO HOMOGENEIZAR LOS TIPOS DE DATOS QUE USA

void printTime() {
    ncPrint("[Time]: ");
    switch (getTimeFragment(6)) {
        case 1:
            ncPrint("Domingo, ");
            break;
        case 2:
            ncPrint("Lunes, ");
            break;
        case 3:
            ncPrint("Martes, ");
            break;
        case 4:
            ncPrint("Miercoles, ");
            break;
        case 5:
            ncPrint("Jueves, ");
            break;
        case 6:
            ncPrint("Viernes, ");
            break;
        case 7:
            ncPrint("Sabado, ");
            break;
        default:
            break;
    }

    ncPrintHex(getTimeFragment(7));// día del mes
    ncPrint("/");
    ncPrintHex(getTimeFragment(8));// mes
    ncPrint("/");
    ncPrintHex(getTimeFragment(9));// año

    ncPrint(" ");

    ncPrintHex(getTimeFragment(4));// hora
    ncPrint(":");
    ncPrintHex(getTimeFragment(2));// minutos
    ncPrint(":");
    ncPrintHex(getTimeFragment(0));// segundos
    ncNewline();
}

int64_t get_seconds(){
    return rtcTypeToDecimal(getTimeFragment(SECONDS));
}

int64_t get_minutes(){
    return rtcTypeToDecimal(getTimeFragment(MINUTES));
}

int64_t get_hours(){
    return rtcTypeToDecimal(getTimeFragment(HOURS));
}

int64_t get_day(){
    return rtcTypeToDecimal(getTimeFragment(DAY));
}

int64_t get_month(){
    return rtcTypeToDecimal(getTimeFragment(MONTH));
}

int64_t get_year(){
    return rtcTypeToDecimal(getTimeFragment(YEAR));
}


// https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
unsigned int rtcTypeToDecimal(unsigned char time){
    return (unsigned int) ((time >> 4) * 10 + (time & 0x0F));
}

void set_time(time_t * time){
    time->seconds = get_seconds();
    time->minutes = get_minutes();
    time->hours = get_hours();
    time->day = get_day();
    time->month = get_month();
    time->year = get_year();
}
