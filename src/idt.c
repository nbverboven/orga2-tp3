/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"


idt_entry idt[255] = { 
};

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/

/*
#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x00;                                                                  \
    idt[numero].attr = (unsigned short) 0x0000;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);
*/

#define INTERRUPT_GATE_KERNEL    0x8E00
#define INTERRUPT_GATE_USUARIO   0xEE00

#define IDT_ENTRY(numero, tipo_puerta)                                                                           \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x0040;                                                                \
    idt[numero].attr = (unsigned short) tipo_puerta;                                                             \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);


void idt_inicializar()
{
    IDT_ENTRY( 0,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 1,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 2,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 3,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 4,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 5,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 6,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 7,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 8,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 9,  INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 10, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 11, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 12, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 13, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 14, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 16, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 17, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 18, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 19, INTERRUPT_GATE_KERNEL  );
    IDT_ENTRY( 32, INTERRUPT_GATE_KERNEL  ); // Interrupción del reloj
    IDT_ENTRY( 33, INTERRUPT_GATE_KERNEL  ); // Interrupción del teclado
    IDT_ENTRY( 102, INTERRUPT_GATE_USUARIO ); // Syscall mover
}
