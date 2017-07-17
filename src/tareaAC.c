/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h" 

void task() {
    /* Tarea */
    int i;
    /*for(i=0;i<10;i++)
    syscall_mover(ADE);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<5;i++)
    syscall_mover(ATR);
    for(i=0;i<5;i++)
    syscall_mover(IZQ);
    for(i=0;i<500;i++)
    syscall_mover(ATR);*/

    for(i=0;i<20;i++)
    syscall_mover(IZQ); 
    for(i=0;i<4;i++)
    syscall_mover(ADE); 
    for(i=0;i<4;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(IZQ);
    for(i=0;i<2;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<8;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<7;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<7;i++)
    syscall_mover(ADE);

    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<3;i++)
    syscall_mover(ADE);
    for(i=0;i<2;i++)
    syscall_mover(IZQ);
    for(i=0;i<1;i++)
    syscall_mover(ADE);
    for(i=0;i<2;i++)
    syscall_mover(DER);
    for(i=0;i<3;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<3;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<7;i++)
    syscall_mover(ADE);

    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<20;i++)
    syscall_mover(DER);
    for(i=0;i<6;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<7;i++)
    syscall_mover(ADE);
    for(i=0;i<20;i++)
    syscall_mover(IZQ);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    for(i=0;i<4;i++)
    syscall_mover(ATR);
    for(i=0;i<10;i++)
    syscall_mover(DER);
    for(i=0;i<4;i++)
    syscall_mover(ADE);
    
    
  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}