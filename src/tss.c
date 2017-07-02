/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

// Índices en la gdt del descriptor de la proxima tss libre para cada jugador
unsigned int proxima_tss_libre_A;
unsigned int proxima_tss_libre_B;

// Índices en la lista de tareas de cada jugador de la siguiente que será lanzada
unsigned int proxima_tarea_a_lanzar_A;
unsigned int proxima_tarea_a_lanzar_B;





void tss_inicializar()
{
	proxima_tss_libre_A = GDT_TSS_TAREA_A_0_DESC;
	proxima_tss_libre_B = GDT_TSS_TAREA_B_0_DESC;
	proxima_tarea_a_lanzar_A = 0;
	proxima_tarea_a_lanzar_B = 0;

	// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_0_15 =    (unsigned int) &tss_inicial & 0x0000FFFF;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_23_16 = ( (unsigned int) &tss_inicial & 0x00FF0000 ) >> 16;
	gdt[GDT_TSS_TAREA_INICIAL_DESC].base_31_24 = ( (unsigned int) &tss_inicial & 0xFF000000 ) >> 24;
}


void tss_inicializar_idle()
{
	// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
	//tss* base_tarea_idle = &tss_idle;
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
	int jug = ( jugador == JUGADOR_A ) ? 0 : 1;

	// Chequeo que no se haya alcanzado el número máximo de tareas por jugador
	if ( ( jug && proxima_tarea_a_lanzar_B < 8 ) || ( !jug && proxima_tarea_a_lanzar_A < 8 ) )
	{
		// Inicializo el TSS de la tarea para el jugador correspondiente
		if ( !jug )
		{
			// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
			gdt[proxima_tss_libre_A].base_0_15  =   (unsigned int) (&tss_zombisA[proxima_tarea_a_lanzar_A]) & 0x0000FFFF;
			gdt[proxima_tss_libre_A].base_23_16 = ( (unsigned int) (&tss_zombisA[proxima_tarea_a_lanzar_A]) & 0x00FF0000 ) >> 16;
			gdt[proxima_tss_libre_A].base_31_24 = ( (unsigned int) (&tss_zombisA[proxima_tarea_a_lanzar_A]) & 0xFF000000 ) >> 24;

			tss_zombisA[proxima_tarea_a_lanzar_A].cr3    = (unsigned int) mmu_inicializar_dir_zombi(codigo_zombie, jugador, posicion_x, posicion_y);
			tss_zombisA[proxima_tarea_a_lanzar_A].eip    = DIR_VIRTUAL_MAPA;
			tss_zombisA[proxima_tarea_a_lanzar_A].ebp    = DIR_VIRTUAL_MAPA + 0x1000;
			tss_zombisA[proxima_tarea_a_lanzar_A].esp    = DIR_VIRTUAL_MAPA + 0x1000;
			tss_zombisA[proxima_tarea_a_lanzar_A].eflags = 0x00000202;
			tss_zombisA[proxima_tarea_a_lanzar_A].cs     = SELECTOR_CODIGO_LVL3;
			tss_zombisA[proxima_tarea_a_lanzar_A].ds     = SELECTOR_DATOS_LVL3;
			tss_zombisA[proxima_tarea_a_lanzar_A].es     = SELECTOR_DATOS_LVL3;
			tss_zombisA[proxima_tarea_a_lanzar_A].gs     = SELECTOR_DATOS_LVL3;
			tss_zombisA[proxima_tarea_a_lanzar_A].fs     = SELECTOR_VIDEO;
			tss_zombisA[proxima_tarea_a_lanzar_A].ss     = SELECTOR_DATOS_LVL3;
			tss_zombisA[proxima_tarea_a_lanzar_A].esp0   = mmu_proxima_pagina_fisica_libre() + 0x1000;
			tss_zombisA[proxima_tarea_a_lanzar_A].ss0    = SELECTOR_DATOS_LVL0;

			tss_zombisA[proxima_tarea_a_lanzar_A].eax    = 0;
			tss_zombisA[proxima_tarea_a_lanzar_A].ebx    = 0;
			tss_zombisA[proxima_tarea_a_lanzar_A].ecx    = 0;
			tss_zombisA[proxima_tarea_a_lanzar_A].edx    = 0;

			proxima_tarea_a_lanzar_A += 1;
			proxima_tss_libre_A += 1; 
		}
		else
		{
			// Actualizo el comienzo del TSS de la tarea idle en su descriptor de la GDT
			gdt[proxima_tss_libre_B].base_0_15 =    (unsigned int) (&tss_zombisB[proxima_tarea_a_lanzar_B]) & 0x0000FFFF;
			gdt[proxima_tss_libre_B].base_23_16 = ( (unsigned int) (&tss_zombisB[proxima_tarea_a_lanzar_B]) & 0x00FF0000 ) >> 16;
			gdt[proxima_tss_libre_B].base_31_24 = ( (unsigned int) (&tss_zombisB[proxima_tarea_a_lanzar_B]) & 0xFF000000 ) >> 24;

			tss_zombisB[proxima_tarea_a_lanzar_B].cr3    = (unsigned int) mmu_inicializar_dir_zombi(codigo_zombie, jugador, posicion_x, posicion_y);;
			tss_zombisB[proxima_tarea_a_lanzar_B].eip    = DIR_VIRTUAL_MAPA;
			tss_zombisB[proxima_tarea_a_lanzar_B].ebp    = DIR_VIRTUAL_MAPA + 0x1000;
			tss_zombisB[proxima_tarea_a_lanzar_B].esp    = DIR_VIRTUAL_MAPA + 0x1000;
			tss_zombisB[proxima_tarea_a_lanzar_B].eflags = 0x00000202;
			tss_zombisB[proxima_tarea_a_lanzar_B].cs     = SELECTOR_CODIGO_LVL3;
			tss_zombisB[proxima_tarea_a_lanzar_B].ds     = SELECTOR_DATOS_LVL3;
			tss_zombisB[proxima_tarea_a_lanzar_B].es     = SELECTOR_DATOS_LVL3;
			tss_zombisB[proxima_tarea_a_lanzar_B].gs     = SELECTOR_DATOS_LVL3;
			tss_zombisB[proxima_tarea_a_lanzar_B].fs     = SELECTOR_VIDEO;
			tss_zombisB[proxima_tarea_a_lanzar_B].ss     = SELECTOR_DATOS_LVL3;
			tss_zombisB[proxima_tarea_a_lanzar_B].esp0   = mmu_proxima_pagina_fisica_libre() + 0x1000;
			tss_zombisB[proxima_tarea_a_lanzar_B].ss0    = SELECTOR_DATOS_LVL0;

			tss_zombisB[proxima_tarea_a_lanzar_B].eax    = 0;
			tss_zombisB[proxima_tarea_a_lanzar_B].ebx    = 0;
			tss_zombisB[proxima_tarea_a_lanzar_B].ecx    = 0;
			tss_zombisB[proxima_tarea_a_lanzar_B].edx    = 0;

			proxima_tarea_a_lanzar_B += 1;
			proxima_tss_libre_B += 1; 		
		}
	}
}
