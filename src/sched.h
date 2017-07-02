/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"

void sched_inicializar();
short sched_proximo_indice();
void sched_handler_teclado(unsigned int tecla);

// typedef enum clases_zombie_e { Guerrero = 0, Mago = 1, Clerigo = 2 } clases_zombie;

typedef struct str_jugador {
	unsigned char  id;
	unsigned short zombies_lanzados;
	unsigned short zombies_restantes;
	unsigned short posicion_x;
	unsigned short posicion_y;
	unsigned short proximo_zombie_a_lanzar;
	unsigned short puntaje_actual;
} __attribute__((__packed__, aligned (8))) jugador;


typedef struct str_info_juego {
	jugador jugador_A;
	jugador jugador_B;
	unsigned char modo_debug_on;
	const char* zombies_disponibles[3];

	
} __attribute__((__packed__, aligned (8))) info_juego;


#endif	/* !__SCHED_H__ */
