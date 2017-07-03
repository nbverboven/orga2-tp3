/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"
#include "game.h"

jugador jugadorA;
jugador jugadorB;
info_juego infoJuego;


void sched_inicializar()
{
	// Inicializo toodas las estructuras del scheduler
	jugadorA.id = 0;
	jugadorA.zombies_lanzados = 0;
	jugadorA.zombies_restantes = 13;
	jugadorA.posicion_x = 0; // Posición inicial (relativa al mapa, no a las dimensiones de la pantalla)
	jugadorA.posicion_y = 21;
	jugadorA.proximo_zombie_a_lanzar = 0; // El primer zombie que tiene disponible para lanzar es de tipo Guerrero
	jugadorA.puntaje_actual = 0;

	jugadorB.id = 1;
	jugadorB.zombies_lanzados = 0;
	jugadorB.zombies_restantes = 13;
	jugadorB.posicion_x = 79;
	jugadorB.posicion_y = 21;
	jugadorB.proximo_zombie_a_lanzar = 0;
	jugadorB.puntaje_actual = 0;

	infoJuego.jugador_A = jugadorA;
	infoJuego.jugador_B = jugadorB;
	infoJuego.modo_debug_on = 0;
	infoJuego.jugador_de_turno = 0; // Empieza el jugador A
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
	const char* i = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
	const char* j = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];

	print( i, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_WHITE | C_BG_RED  );
	print( j, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );
}


unsigned short sched_proximo_indice()
{
	task_info* tareas;
	unsigned char i;
	unsigned char* actual;
	unsigned short res;

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

	infoJuego.jugador_de_turno = !infoJuego.jugador_de_turno; // El próximo turno será del otro jugador

	return res;
}


unsigned short sched_proximo_indice_libre(unsigned int jugador)
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


void sched_handler_teclado(unsigned int tecla)
{
	const char* asd;
	const char* tz;
	unsigned short attr;
	// unsigned short i;

	switch ( tecla )
	{
		/********** Teclas para el jugador A ************/

		// W
		case 0x11:
			tz = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( tz, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_RED | C_BG_RED  );

			// jugadorA.posicion_y = (jugadorA.posicion_y == 1) ? 44 : (jugadorA.posicion_y - 1);
			jugadorA.posicion_y = (jugadorA.posicion_y + 43) % 44;
			print( tz, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// S
		case 0x1F:
			tz = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( tz, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_RED | C_BG_RED  );

			// jugadorA.posicion_y = (jugadorA.posicion_y == 44) ? 1 : (jugadorA.posicion_y + 1);
			jugadorA.posicion_y = (jugadorA.posicion_y + 1) % 44;
			print( tz, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// A
		case 0x1E:
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_RED | C_BG_RED  );

			jugadorA.proximo_zombie_a_lanzar = (jugadorA.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// D
		case 0x20:
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_RED | C_BG_RED  );

			jugadorA.proximo_zombie_a_lanzar = (jugadorA.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// LShift
		case 0x2A:
			asd = (const char*) "LShift";
			attr = ( C_FG_LIGHT_CYAN | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0xAA:
			asd = (const char*) "LShift";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		/************** Fin teclas jugador A **************/


		/************* Teclas para el jugador B ***********/

		// I
		case 0x17:
			tz = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( tz, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.posicion_y = (jugadorB.posicion_y + 43) % 44;
			print( tz, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// K
		case 0x25:
			tz = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( tz, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.posicion_y = (jugadorB.posicion_y + 1) % 44;
			print( tz, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// J
		case 0x24:
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.proximo_zombie_a_lanzar = (jugadorB.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// L
		case 0x26:
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.proximo_zombie_a_lanzar = (jugadorB.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// RShift
		case 0x36:
			asd = (const char*) "RShift";
			attr = ( C_FG_LIGHT_MAGENTA | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		case 0xB6:
			asd = (const char*) "RShift";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		/************** Fin teclas jugador B **************/


		/*************** Tecla modo debug *****************/

		// Y
		case 0x15:
			asd = (const char*) "Y";
			attr = ( C_FG_LIGHT_BROWN | C_BG_BLACK );
			print( asd, 25, 0, attr );
			break;

		case 0x95:
			asd = (const char*) "Y";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 25, 0, attr );
			break;
	}
}


void sched_ejecutar_orden_66(direccion d)
{

}

