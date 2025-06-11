//******************************************************************************
// Archivo: idtLoader.h
//******************************************************************************

#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

#include <stdint.h>

/* Descriptor de interrupcion */
typedef struct {
    uint16_t offset_l, selector;
    uint8_t cero, access;
    uint16_t offset_m;
    uint32_t offset_h, other_cero;
} DESCR_INT;


//******************************************************************************
// DECLARACIÓN DE PROTOTIPOS
//******************************************************************************

void load_idt();

#endif// _IDTLOADER_H_
