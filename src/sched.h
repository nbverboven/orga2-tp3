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

typedef enum zombie_type_e { guerrero = 0, mago = 1, clerigo = 2 } zombie_type;

void sched_inicializar();
unsigned short sched_proximo_indice();
unsigned short sched_proxima_tss_libre(unsigned int);
void sched_desalojar_tarea_actual();


typedef struct str_jugador {
	unsigned short zombies_restantes;
	unsigned short posicion_x;
	unsigned short posicion_y;
	zombie_type    proximo_zombie_a_lanzar;
	unsigned short puntaje_actual;
} __attribute__((__packed__)) jugador;


typedef struct str_task_info {
	unsigned char  esta_activa;
	unsigned short selector_tss;
	zombie_type    z_tipo;
	unsigned short z_posicion_x;
	unsigned short z_posicion_y;
	unsigned char  z_reloj;
} __attribute__((__packed__)) task_info;


typedef struct str_info_juego {
	unsigned char modo_debug_on;
	const char*   zombies_disponibles[3];
	unsigned char jugador_de_turno;
	jugador       jugador_A;
	jugador       jugador_B;
	unsigned char tarea_actual_A;
	unsigned char tarea_actual_B;
	task_info     tareasA[CANT_ZOMBIS];
	task_info     tareasB[CANT_ZOMBIS];
} __attribute__((__packed__)) info_juego;


extern info_juego infoJuego;

#endif	/* !__SCHED_H__ */
