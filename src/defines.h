/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODIGO_LVL_0_DESC   8
#define GDT_IDX_CODIGO_LVL_3_DESC   9
#define GDT_IDX_DATO_LVL_0_DESC     10
#define GDT_IDX_DATO_LVL_3_DESC     11
#define GDT_IDX_DATO_VID_LVL_0_DESC 12

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define DIR_VIRTUAL_MAPA        0x08000000
#define PAGE_DIRECTORY_KERNEL   0x27000
#define PAGE_TABLE_KERNEL       0x28000
#define INICIO_MAPA             0x400000
#define FIN_MAPA                0xDC3FFF

#define DIR_CODIGO_TAREA_A_G    0x10000
#define DIR_CODIGO_TAREA_A_M    0x11000
#define DIR_CODIGO_TAREA_A_C    0x12000
#define DIR_CODIGO_TAREA_B_G    0x13000
#define DIR_CODIGO_TAREA_B_M    0x14000
#define DIR_CODIGO_TAREA_B_C    0x15000
#define DIR_CODIGO_TAREA_IDLE   0x16000


#endif  /* !__DEFINES_H__ */
