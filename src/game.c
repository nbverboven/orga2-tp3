/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

void game_jugador_mover(unsigned int value)
{
	const char* asd;
	const char* tz;
	unsigned short attr;

	switch ( value )
	{
		/********** Teclas para el jugador A ************/

		// W
		case 0x11:
			tz = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( tz, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_RED | C_BG_RED  );

			infoJuego.jugador_A.posicion_y = (infoJuego.jugador_A.posicion_y + 43) % 44;
			print( tz, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// S
		case 0x1F:
			tz = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( tz, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_RED | C_BG_RED  );

			infoJuego.jugador_A.posicion_y = (infoJuego.jugador_A.posicion_y + 1) % 44;
			print( tz, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// A
		case 0x1E:
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_RED | C_BG_RED  );

			infoJuego.jugador_A.proximo_zombie_a_lanzar = (infoJuego.jugador_A.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// D
		case 0x20:
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_RED | C_BG_RED  );

			infoJuego.jugador_A.proximo_zombie_a_lanzar = (infoJuego.jugador_A.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_A.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_A.posicion_x, infoJuego.jugador_A.posicion_y+1, C_FG_WHITE | C_BG_RED  );

			break;

		// LShift
		case 0x2A:
			game_lanzar_zombi(JUGADOR_A);
			break;

		/************** Fin teclas jugador A **************/


		/************* Teclas para el jugador B ***********/

		// I
		case 0x17:
			tz = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( tz, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			infoJuego.jugador_B.posicion_y = (infoJuego.jugador_B.posicion_y + 43) % 44;
			print( tz, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// K
		case 0x25:
			tz = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( tz, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			infoJuego.jugador_B.posicion_y = (infoJuego.jugador_B.posicion_y + 1) % 44;
			print( tz, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// J
		case 0x24:
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			infoJuego.jugador_B.proximo_zombie_a_lanzar = (infoJuego.jugador_B.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// L
		case 0x26:
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_BLUE | C_BG_BLUE  );

			infoJuego.jugador_B.proximo_zombie_a_lanzar = (infoJuego.jugador_B.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[infoJuego.jugador_B.proximo_zombie_a_lanzar];
			print( asd, infoJuego.jugador_B.posicion_x, infoJuego.jugador_B.posicion_y+1, C_FG_WHITE | C_BG_BLUE  );

			break;

		// RShift
		case 0x36:
			game_lanzar_zombi(JUGADOR_B);
			break;

		/************** Fin teclas jugador B **************/


		/*************** Tecla modo debug *****************/

		// Y
		case 0x15:
			infoJuego.modo_debug_on = !infoJuego.modo_debug_on;
			asd = (const char*) "Modo debug on";

			if ( !infoJuego.modo_debug_on )
			{
				attr = ( C_FG_BLACK | C_BG_BLACK );
			}
			else
			{
				attr = ( C_FG_LIGHT_BROWN | C_BG_BLACK );
			}

			print( asd, 35, 0, attr );
			break;
	}
}


void game_lanzar_zombi(unsigned int jugador)
{
	// Primero que nada, veo si el jugador tiene zombies para lanzar
	if ( (!jugador && infoJuego.jugador_A.zombies_restantes > 0) || (jugador && infoJuego.jugador_B.zombies_restantes > 0) )
	{
		task_info* tareas;
		int i = 0;

		// Obtengo el selector del primer tss libre para el jugador
		unsigned short selec = sched_proxima_tss_libre(jugador);

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
		
		if ( i < CANT_ZOMBIS )
		{

			if ( !jugador )
			{
				tareas[i].z_tipo = infoJuego.jugador_A.proximo_zombie_a_lanzar;
				tareas[i].z_posicion_x = 1;
				tareas[i].z_posicion_y = infoJuego.jugador_A.posicion_y;
				tss_inicializar_zombie( game_dame_codigo(infoJuego.jugador_A.proximo_zombie_a_lanzar, JUGADOR_A), 
									    JUGADOR_A, selec, tareas[i].z_posicion_x, tareas[i].z_posicion_y );
				infoJuego.jugador_A.zombies_restantes -= 1;
				tareas[i].esta_activa = 1;
				tareas[i].z_reloj = 0;

				print( infoJuego.zombies_disponibles[tareas[i].z_tipo], tareas[i].z_posicion_x+1, 
					   tareas[i].z_posicion_y+1, C_FG_WHITE | C_BG_RED );
			}
			else
			{
				tareas[i].z_tipo = infoJuego.jugador_B.proximo_zombie_a_lanzar;
				tareas[i].z_posicion_x = 76;
				tareas[i].z_posicion_y = infoJuego.jugador_B.posicion_y;
				tss_inicializar_zombie( game_dame_codigo(infoJuego.jugador_B.proximo_zombie_a_lanzar, JUGADOR_B), 
									    JUGADOR_B, selec, tareas[i].z_posicion_x, tareas[i].z_posicion_y );
				infoJuego.jugador_B.zombies_restantes -= 1;
				tareas[i].esta_activa = 1;
				tareas[i].z_reloj = 0;

				print( infoJuego.zombies_disponibles[tareas[i].z_tipo], tareas[i].z_posicion_x+1, 
					   tareas[i].z_posicion_y+1, C_FG_WHITE | C_BG_BLUE );
			}
		}	
	}
}


void game_move_current_zombi(direccion dir)
{
	unsigned char j = infoJuego.jugador_de_turno;
	unsigned char i;
	task_info* actual; // Puntero al zombie que llamó a mover

	if ( !j )
	{
		i = infoJuego.tarea_actual_A;
		actual = (task_info*) &(infoJuego.tareasA[i]);
	}
	else
	{
		i = infoJuego.tarea_actual_B;
		actual = (task_info*) &(infoJuego.tareasB[i]);
	}

	print( "x", actual->z_posicion_x+1, actual->z_posicion_y+1, C_FG_LIGHT_GREY | C_BG_GREEN );

	unsigned int CR3 = rcr3();

	switch ( dir )
	{
		case IZQ:
			actual->z_posicion_y = ((actual->z_posicion_y)+43+2*j)%44;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( 0x08004000, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case DER:
			actual->z_posicion_y = ((actual->z_posicion_y)+1+42*j)%44;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( 0x08005000, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case ADE:
			actual->z_posicion_x = (actual->z_posicion_x)+1-2*j;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( 0x08006000, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case ATR:
			actual->z_posicion_x = (actual->z_posicion_x)-1+2*j;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( 0x08001000, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;
	}

	if ( !j )
	{
		print( infoJuego.zombies_disponibles[actual->z_tipo], actual->z_posicion_x+1, 
			   actual->z_posicion_y+1, C_FG_WHITE | C_BG_RED );
	}
	else
	{
		print( infoJuego.zombies_disponibles[actual->z_tipo], actual->z_posicion_x+1, 
			   actual->z_posicion_y+1, C_FG_WHITE | C_BG_BLUE );
	}
}


unsigned int game_dame_codigo(zombie_type tipo, unsigned char j)
{
	unsigned int res;

	if ( !j )
	{
		switch ( tipo )
		{
			case guerrero:
				res = DIR_CODIGO_TAREA_A_G;
				break;

			case mago:
				res = DIR_CODIGO_TAREA_A_M;
				break;

			case clerigo:
				res = DIR_CODIGO_TAREA_A_C;
				break;
		}
	}
	else
	{
		switch ( tipo )
		{
			case guerrero:
				res = DIR_CODIGO_TAREA_B_G;
				break;

			case mago:
				res = DIR_CODIGO_TAREA_B_M;
				break;

			case clerigo:
				res = DIR_CODIGO_TAREA_B_C;
				break;
		}
	}

	return res;
}