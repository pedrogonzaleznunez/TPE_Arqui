#include <defs.h>
#include <interrupts.h>
#include <syscalls.h>

#pragma pack(push) /* Push de la alineación actual */
#pragma pack(1)    /* Alinear las siguiente estructuras a 1 byte */

static void setup_IDT_entry(int index, uint64_t offset);


#pragma pack(pop) /* Reestablece la alinceación actual */

DESCR_INT *idt = (DESCR_INT *) 0;// IDT de 255 entradas

// tabla idt
void load_idt() {
    // interrupciones propias
    setup_IDT_entry(0x00, (uint64_t) &_exception00Handler);
    setup_IDT_entry(0x06, (uint64_t) &_exception06Handler);

    setup_IDT_entry(0x20, (uint64_t) &_irq00Handler);
    setup_IDT_entry(0x21, (uint64_t) &_irq01Handler);

    // syscalls propias
    setup_IDT_entry(0x80, (uint64_t) &_syscallHandler);


    // Solo interrupcion timer tick habilitadas y teclado
    picMasterMask(0xFC);// 1111 1100
    picSlaveMask(0xFF); // irq 0 y irq 1 habilitadas

    _sti();
}

static void setup_IDT_entry(int index, uint64_t offset) {
    idt[index].selector = 0x08;
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].access = ACS_INT;
    idt[index].cero = 0;
    idt[index].other_cero = (uint64_t) 0;
}
