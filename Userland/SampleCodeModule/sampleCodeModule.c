/* sampleCodeModule.c */

#include <shell.h>

char *v = (char *) 0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {

    shell();

    if (var1 == 0 && var2 == 0) return 0xDEADC0DE;

    return 0xDEADBEEF;
}
