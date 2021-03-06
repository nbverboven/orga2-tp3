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
#define NULL                    0


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8
#define ZOMBIES_A_LANZAR        11

#define SIZE_W                  78
#define SIZE_H                  44
#define TAM_PAGINA              0x1000
#define JUGADOR_A               0
#define JUGADOR_B               1

#define SELECTOR_CODIGO_LVL0      0x0040
#define SELECTOR_DATOS_LVL0       0x0050
#define SELECTOR_CODIGO_LVL3      0x004B // 0x004B = 0000 0000 0100 1011. índice = 0000000001001 (9)  gdt/ldt = 0 dpl = 11 (3)
#define SELECTOR_DATOS_LVL3       0x005B // 0x005B = 0000 0000 0101 1011. índice = 0000000001011 (11) gdt/ldt = 0 dpl = 11 (3)
#define SELECTOR_VIDEO            0x0060


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 31

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODIGO_LVL_0_DESC   8
#define GDT_IDX_CODIGO_LVL_3_DESC   9
#define GDT_IDX_DATO_LVL_0_DESC     10
#define GDT_IDX_DATO_LVL_3_DESC     11
#define GDT_IDX_DATO_VID_LVL_0_DESC 12

#define GDT_TSS_TAREA_INICIAL_DESC  13
#define GDT_TSS_TAREA_IDLE_DESC     14

#define GDT_TSS_TAREA_A_0_DESC      15
#define GDT_TSS_TAREA_A_1_DESC      16
#define GDT_TSS_TAREA_A_2_DESC      17
#define GDT_TSS_TAREA_A_3_DESC      18
#define GDT_TSS_TAREA_A_4_DESC      19
#define GDT_TSS_TAREA_A_5_DESC      20
#define GDT_TSS_TAREA_A_6_DESC      21
#define GDT_TSS_TAREA_A_7_DESC      22

#define GDT_TSS_TAREA_B_0_DESC      23
#define GDT_TSS_TAREA_B_1_DESC      24
#define GDT_TSS_TAREA_B_2_DESC      25
#define GDT_TSS_TAREA_B_3_DESC      26
#define GDT_TSS_TAREA_B_4_DESC      27
#define GDT_TSS_TAREA_B_5_DESC      28
#define GDT_TSS_TAREA_B_6_DESC      29
#define GDT_TSS_TAREA_B_7_DESC      30

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define DIR_VIRTUAL_MAPA        0x08000000
#define PILA_KERNEL             0x27000
#define PAGE_DIRECTORY_KERNEL   0x27000
#define PAGE_TABLE_KERNEL       0x28000
#define INICIO_MAPA             0x400000

#define DIR_CODIGO_TAREA_A_G    0x10000
#define DIR_CODIGO_TAREA_A_M    0x11000
#define DIR_CODIGO_TAREA_A_C    0x12000
#define DIR_CODIGO_TAREA_B_G    0x13000
#define DIR_CODIGO_TAREA_B_M    0x14000
#define DIR_CODIGO_TAREA_B_C    0x15000
#define DIR_CODIGO_TAREA_IDLE   0x16000


#endif  /* !__DEFINES_H__ */
