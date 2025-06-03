/* sampleCodeModule.c */

#include "./include/stdio.h"
#include "pongis.h"
//#include "./include/syscalls.h" --> incluido en pongis.h

char *v = (char *) 0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {

   while (1) {

        startGame();
        return 0;
        
   }
   
    if (var1 == 0 && var2 == 0) return 0xDEADC0DE;

    return 0xDEADBEEF;
}