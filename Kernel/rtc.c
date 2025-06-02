#include <naiveConsole.h>
#include <rtc.h>

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
