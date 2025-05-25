/* sampleCodeModule.c */

#include "./include/syscalls.h"
// #include <stdint.h>

//Kernel/include/videoDriver.h
// extern int32_t sys_call(int64_t id, ...);

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	// All the following code may be removed 
	// *v = 'X';
	// *(v+1) = 0x74;
	
	int64_t fd = 0;
	// const char * buf = "Hola mundo?";
	int64_t count;
	// sys_call(0, fd, buf, count);

	char buffer[20];
	count = 15;

	sys_call(1, fd, buffer, count);

	const char * s = "termino la sys 1";
	
	sys_call(0, fd, s, count);

	fd = 1;
	sys_call(0, fd, (const char *) buffer, count);

	// while (1);	
	
	// Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}