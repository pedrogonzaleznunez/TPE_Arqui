include ../../Kernel/Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(OBJECTS) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

syscall.o:
	mkdir -p asm
	$(ASM) $(ASMFLAGS) asm/syscallsUser.asm -o asm/syscallsUser.o

clean:
	rm -rf *.o asm/*.o

.PHONY: all clean
