/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

info_juego infoJuego;


void sched_inicializar()
{
	// Inicializo toodas las estructuras del scheduler
	infoJuego.jugador_A.zombies_restantes = ZOMBIES_A_LANZAR;
	infoJuego.jugador_A.posicion_x = 0; // Posición inicial (relativa al mapa, no a las dimensiones de la pantalla)
	infoJuego.jugador_A.posicion_y = 21;
	infoJuego.jugador_A.proximo_zombie_a_lanzar = guerrero;
	infoJuego.jugador_A.puntaje_actual = 0;

	infoJuego.jugador_B.zombies_restantes = ZOMBIES_A_LANZAR;
	infoJuego.jugador_B.posicion_x = 79;
	infoJuego.jugador_B.posicion_y = 21;
	infoJuego.jugador_B.proximo_zombie_a_lanzar = guerrero;
	infoJuego.jugador_B.puntaje_actual = 0;

	infoJuego.jugador_de_turno = 1; // Cuando se busque a primera tarea, esto cambia al jugador A
	infoJuego.tarea_actual_A = 0;
	infoJuego.tarea_actual_B = 0;
	infoJuego.zombies_disponibles[0] = "G"; // Guerrero
	infoJuego.zombies_disponibles[1] = "M"; // Mago
	infoJuego.zombies_disponibles[2] = "C"; // Clerigo

	// Cargo los selectores de tss de las tareas
	int gdtA = GDT_TSS_TAREA_A_0_DESC;
	int gdtB = GDT_TSS_TAREA_B_0_DESC;

	for (int i = 0; i < CANT_ZOMBIS; ++i)
	{
		infoJuego.tareasA[i].esta_activa = 0;
		infoJuego.tareasB[i].esta_activa = 0;
		infoJuego.tareasA[i].selector_tss = gdtA << 3;
		infoJuego.tareasB[i].selector_tss = gdtB << 3;

		++gdtA;
		++gdtB;
	}

	// Inicializo los jugadores y su información en la pantalla
	const char* i = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
	const char* j = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];

	print( i, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_WHITE | C_BG_RED  );
	print( j, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

	inicializar_pagina_video();
}


unsigned short sched_proximo_indice()
{
	task_info* tareas;
	unsigned char i;
	unsigned char* actual;
	unsigned short res;
	infoJuego.jugador_de_turno = !infoJuego.jugador_de_turno;

	// Veo si es el turno del jugador A o del B
	if ( !infoJuego.jugador_de_turno )
	{
		tareas = (task_info*) &infoJuego.tareasA;
		actual = (unsigned char*) &infoJuego.tarea_actual_A;
		i = (infoJuego.tarea_actual_A + 1) % 8;
	}
	else
	{
		tareas = (task_info*) &infoJuego.tareasB;
		actual = (unsigned char*) &infoJuego.tarea_actual_B;
		i = (infoJuego.tarea_actual_B + 1) % 8;
	}

	// Busco la siguiente tarea activa
	while ( i != *actual && !tareas[i].esta_activa )
	{
		i = (i + 1) % 8;
	}

	// Chequeo que haya una tarea activa (puede ser la actual o no)
	if ( tareas[i].esta_activa )
	{
		// Actualizo la tarea actual y devuelvo su selector de tss
		*actual = i;
		res = tareas[i].selector_tss;
	}
	else
	{
		res = 0;
	}

	return res;
}


unsigned short sched_proxima_tss_libre(unsigned int jugador)
{
	task_info* tareas;
	int i = 0;
	unsigned short res;

	// Busco en la lista de tareas del jugador correspondiente
	if ( !jugador )
	{
		tareas = (task_info*) &infoJuego.tareasA;
	}
	else
	{
		tareas = (task_info*) &infoJuego.tareasB;
	}

	while ( i < CANT_ZOMBIS && tareas[i].esta_activa )
	{
		++i;
	}

	// Si llegué hasta el final es porque no había un tss libre
	if ( i == CANT_ZOMBIS )
	{
		res = 0;
	}
	else
	{
		res = tareas[i].selector_tss;
	}

	return res;
}


void sched_desalojar_tarea_actual()
{
	// Busco el jugador cuya tarea podujo la excepción
	unsigned char j = infoJuego.jugador_de_turno;
	unsigned int x;
	unsigned int y;

	if ( j == JUGADOR_A )
	{
		infoJuego.tareasA[infoJuego.tarea_actual_A].esta_activa = 0; // Marco la tarea como inactiva
		x = infoJuego.tareasA[infoJuego.tarea_actual_A].z_posicion_x;
		y = infoJuego.tareasA[infoJuego.tarea_actual_A].z_posicion_y;
		print( "X", x+1, y+1, C_FG_RED | C_BG_GREEN );
	}
	else
	{
		infoJuego.tareasB[infoJuego.tarea_actual_B].esta_activa = 0;
		x = infoJuego.tareasB[infoJuego.tarea_actual_B].z_posicion_x;
		y = infoJuego.tareasB[infoJuego.tarea_actual_B].z_posicion_y;
		print( "X", x+1, y+1, C_FG_BLUE | C_BG_GREEN );
	}

	// Asigno puntos si corresponde
	if ( x == 0 )
	{
		infoJuego.jugador_B.puntaje_actual += 1;
	}

	if ( x == 77 )
	{
		infoJuego.jugador_A.puntaje_actual += 1;
	}
}