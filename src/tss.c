/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "sched.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

// Índices en la gdt del descriptor de la proxima tss libre para cada jugador
unsigned int proxima_tss_libre_A;
unsigned int proxima_tss_libre_B;


void tss_inicializar()
{
	// Actualizo el comienzo del TSS de las tareas en su descriptor de la GDT
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_0_15 =    (unsigned int) &tss_inicial & 0x0000FFFF;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_23_16 = ( (unsigned int) &tss_inicial & 0x00FF0000 ) >> 16;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_31_24 = ( (unsigned int) &tss_inicial & 0xFF000000 ) >> 24;

	int i = GDT_TSS_TAREA_A_0_DESC;
	int j = GDT_TSS_TAREA_B_0_DESC;

	for (int k = 0; k < CANT_ZOMBIS; ++k)
	{
		gdt[i].base_0_15  =   (unsigned int) (&tss_zombisA[k]) & 0x0000FFFF;
		gdt[i].base_23_16 = ( (unsigned int) (&tss_zombisA[k]) & 0x00FF0000 ) >> 16;
		gdt[i].base_31_24 = ( (unsigned int) (&tss_zombisA[k]) & 0xFF000000 ) >> 24;

		gdt[j].base_0_15  =   (unsigned int) (&tss_zombisB[k]) & 0x0000FFFF;
		gdt[j].base_23_16 = ( (unsigned int) (&tss_zombisB[k]) & 0x00FF0000 ) >> 16;
		gdt[j].base_31_24 = ( (unsigned int) (&tss_zombisB[k]) & 0xFF000000 ) >> 24;

		++i;
		++j;
	}
}


void tss_inicializar_idle()
{
	// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_0_15 =    (unsigned int) &tss_idle & 0x0000FFFF;
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_23_16 = ( (unsigned int) &tss_idle & 0x00FF0000 ) >> 16;
	gdt[GDT_TSS_TAREA_IDLE_DESC].base_31_24 = ( (unsigned int) &tss_idle & 0xFF000000 ) >> 24;

	// Inicializo el TSS de la tarea idle
	tss_idle.cr3    = PAGE_DIRECTORY_KERNEL;
	tss_idle.eip    = DIR_CODIGO_TAREA_IDLE;
	tss_idle.ebp    = PILA_KERNEL;
	tss_idle.esp    = PILA_KERNEL;
	tss_idle.eflags = 0x00000202;
	tss_idle.cs     = SELECTOR_CODIGO_LVL0;
	tss_idle.ds     = SELECTOR_DATOS_LVL0;
	tss_idle.es     = SELECTOR_DATOS_LVL0;
	tss_idle.gs     = SELECTOR_DATOS_LVL0;
	tss_idle.fs     = SELECTOR_VIDEO;
	tss_idle.ss     = SELECTOR_DATOS_LVL0;

	tss_idle.eax    = 0;
	tss_idle.ebx    = 0;
	tss_idle.ecx    = 0;
	tss_idle.edx    = 0;
}


void tss_inicializar_zombie(unsigned int codigo_zombie, unsigned int jugador,
                            unsigned int posicion_x, unsigned int posicion_y)
{
	unsigned short i = sched_proximo_indice();
	tss* tss_zombie;

	if ( jugador == JUGADOR_A )
	{
		tss_zombie = (tss*) &tss_zombisA;
	}
	else
	{
		tss_zombie = (tss*) &tss_zombisB;	
	}

	// Inicializo el TSS de la tarea para el jugador correspondiente
	tss_zombie[i].cr3    = (unsigned int) mmu_inicializar_dir_zombi(codigo_zombie, jugador, posicion_x, posicion_y);
	tss_zombie[i].eip    = DIR_VIRTUAL_MAPA;
	tss_zombie[i].ebp    = DIR_VIRTUAL_MAPA + 0x1000;
	tss_zombie[i].esp    = DIR_VIRTUAL_MAPA + 0x1000;
	tss_zombie[i].eflags = 0x00000202;
	tss_zombie[i].cs     = SELECTOR_CODIGO_LVL3;
	tss_zombie[i].ds     = SELECTOR_DATOS_LVL3;
	tss_zombie[i].es     = SELECTOR_DATOS_LVL3;
	tss_zombie[i].gs     = SELECTOR_DATOS_LVL3;
	tss_zombie[i].fs     = SELECTOR_VIDEO;
	tss_zombie[i].ss     = SELECTOR_DATOS_LVL3;
	tss_zombie[i].esp0   = mmu_proxima_pagina_fisica_libre() + 0x1000;
	tss_zombie[i].ss0    = SELECTOR_DATOS_LVL0;

	tss_zombie[i].eax    = 0;
	tss_zombie[i].ebx    = 0;
	tss_zombie[i].ecx    = 0;
	tss_zombie[i].edx    = 0;
}