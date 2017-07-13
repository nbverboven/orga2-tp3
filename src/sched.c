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
	jugadorA.zombies_lanzados = 0;
	jugadorA.zombies_restantes = 13;
	jugadorA.posicion_x = 0; // Posición inicial (relativa al mapa, no a las dimensiones de la pantalla)
	jugadorA.posicion_y = 21;
	jugadorA.proximo_zombie_a_lanzar = guerrero;
	jugadorA.puntaje_actual = 0;

	jugadorB.zombies_lanzados = 0;
	jugadorB.zombies_restantes = 13;
	jugadorB.posicion_x = 79;
	jugadorB.posicion_y = 21;
	jugadorB.proximo_zombie_a_lanzar = guerrero;
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


void sched_lanzar_zombie(unsigned int jugador)
{
	task_info* tareas;
	int i = 0;

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
			tareas[i].z_posicion_x = 0;
			tareas[i].z_posicion_y = infoJuego.jugador_A.posicion_y;
			tss_inicializar_zombie( sched_dame_codigo(infoJuego.jugador_A.proximo_zombie_a_lanzar, JUGADOR_A), 
								    JUGADOR_A, tareas[i].z_posicion_x, tareas[i].z_posicion_y );
			infoJuego.jugador_A.zombies_lanzados += 1;
			infoJuego.jugador_A.zombies_restantes -= 1;
			tareas[i].esta_activa = 1;

			print( infoJuego.zombies_disponibles[tareas[i].z_tipo], tareas[i].z_posicion_x+1, 
				   tareas[i].z_posicion_y+1, C_FG_WHITE | C_BG_RED );
		}
		else
		{
			tareas[i].z_tipo = infoJuego.jugador_B.proximo_zombie_a_lanzar;
			tareas[i].z_posicion_x = 77;
			tareas[i].z_posicion_y = infoJuego.jugador_B.posicion_y;
			tss_inicializar_zombie( sched_dame_codigo(infoJuego.jugador_B.proximo_zombie_a_lanzar, JUGADOR_B), 
								    JUGADOR_B, tareas[i].z_posicion_x, tareas[i].z_posicion_y );
			infoJuego.jugador_B.zombies_lanzados += 1;
			infoJuego.jugador_B.zombies_restantes -= 1;
			tareas[i].esta_activa = 1;

			print( infoJuego.zombies_disponibles[tareas[i].z_tipo], tareas[i].z_posicion_x+1, 
				   tareas[i].z_posicion_y+1, C_FG_WHITE | C_BG_BLUE );
		}
	}
}


void sched_handler_teclado(unsigned int tecla)
{
	const char* asd;
	const char* tz;
	unsigned short attr;

	switch ( tecla )
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
			sched_lanzar_zombie(JUGADOR_A);
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
			sched_lanzar_zombie(JUGADOR_B);
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
	unsigned int codigo_z = sched_dame_codigo( actual->z_tipo, infoJuego.jugador_de_turno );

	switch ( d )
	{
		case IZQ:
			actual->z_posicion_y = ((actual->z_posicion_y)+43+2*j)%44;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( codigo_z, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case DER:
			actual->z_posicion_y = ((actual->z_posicion_y)+1+42*j)%44;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( codigo_z, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case ADE:
			actual->z_posicion_x = (actual->z_posicion_x)+1-2*j;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( codigo_z, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
			break;

		case ATR:
			actual->z_posicion_x = (actual->z_posicion_x)-1+2*j;
			mmu_desmapear_paginas_zombie( CR3 );
			mmu_mapear_paginas_zombie( codigo_z, j, CR3, actual->z_posicion_x, actual->z_posicion_y );
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


// Toma como argumento un selector de tss
tss* sched_dame_tss(unsigned short selector)
{
	gdt_entry* aux = (gdt_entry*) &gdt[selector >> 3];
	tss* res = (tss*) ((unsigned int) aux->base_0_15 | ((unsigned int) aux->base_23_16) << 16  | ((unsigned int) aux->base_31_24) << 24 );
	return res;
}


unsigned int sched_dame_codigo(zombie_type tipo, unsigned char j)
{
	unsigned int res;

	if ( !j )
	{
		if ( tipo == guerrero )
		{
			res = DIR_CODIGO_TAREA_A_G;
		}
		else
		{
			if ( tipo == mago )
			{
				res = DIR_CODIGO_TAREA_A_M;
			}
			else
			{
				res = DIR_CODIGO_TAREA_A_C;
			}
		}
	}
	else
	{
		if ( tipo == guerrero )
		{
			res = DIR_CODIGO_TAREA_B_G;
		}
		else
		{
			if ( tipo == mago )
			{
				res = DIR_CODIGO_TAREA_B_M;
			}
			else
			{
				res = DIR_CODIGO_TAREA_B_C;
			}
		}
	}

	return res;
}
