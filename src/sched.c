/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

jugador jugadorA;
jugador jugadorB;
info_juego infoJuego;

void sched_inicializar()
{
	jugadorA.id = 0;
	jugadorA.zombies_lanzados = 0;
	jugadorA.zombies_restantes = 8;
	jugadorA.posicion_x = 0;
	jugadorA.posicion_y = 22;
	jugadorA.proximo_zombie_a_lanzar = 0;

	jugadorB.id = 1;
	jugadorB.zombies_lanzados = 0;
	jugadorB.zombies_restantes = 8;
	jugadorB.posicion_x = 79;
	jugadorB.posicion_y = 22;
	jugadorB.proximo_zombie_a_lanzar = 0;

	infoJuego.jugador_A = jugadorA;
	infoJuego.jugador_B = jugadorB;
	infoJuego.modo_debug_on = 0;
	infoJuego.zombies_disponibles[0] = "G"; // Guerrero
	infoJuego.zombies_disponibles[1] = "M"; // Mago
	infoJuego.zombies_disponibles[2] = "C"; // Clerigo

	const char* i = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
	const char* j = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];

	print( i, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_WHITE | C_BG_RED  );
	print( j, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_WHITE | C_BG_BLUE  );
}

unsigned short sched_proximo_indice()
{
	return 0;
}

void sched_handler_teclado(unsigned int tecla)
{
	const char* asd;
	const char* pos;
	unsigned short attr;
	// unsigned short i;

	switch ( tecla )
	{
		/********** Teclas para el jugador A ************/

		// W
		case 0x11:
			pos = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( pos, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_RED | C_BG_RED  );

			jugadorA.posicion_y = (jugadorA.posicion_y == 1) ? 44 : (jugadorA.posicion_y - 1);
			print( pos, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_WHITE | C_BG_RED  );

			break;

		// S
		case 0x1F:
			pos = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( pos, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_RED | C_BG_RED  );

			jugadorA.posicion_y = (jugadorA.posicion_y == 44) ? 1 : (jugadorA.posicion_y + 1);
			print( pos, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_WHITE | C_BG_RED  );

			break;

		// A
		case 0x1E:
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_RED | C_BG_RED  );

			jugadorA.proximo_zombie_a_lanzar = (jugadorA.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_WHITE | C_BG_RED  );

			break;

		// D
		case 0x20:
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_RED | C_BG_RED  );

			jugadorA.proximo_zombie_a_lanzar = (jugadorA.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[jugadorA.proximo_zombie_a_lanzar];
			print( asd, jugadorA.posicion_x, jugadorA.posicion_y, C_FG_WHITE | C_BG_RED  );

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
			pos = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( pos, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.posicion_y = (jugadorB.posicion_y == 1) ? 44 : (jugadorB.posicion_y - 1);
			print( pos, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_WHITE | C_BG_BLUE  );

			break;

		// K
		case 0x25:
			pos = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( pos, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.posicion_y = (jugadorB.posicion_y == 44) ? 1 : (jugadorB.posicion_y + 1);
			print( pos, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_WHITE | C_BG_BLUE  );

			break;

		// J
		case 0x24:
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.proximo_zombie_a_lanzar = (jugadorB.proximo_zombie_a_lanzar + 2) % 3;
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_WHITE | C_BG_BLUE  );

			break;

		// L
		case 0x26:
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_BLUE | C_BG_BLUE  );

			jugadorB.proximo_zombie_a_lanzar = (jugadorB.proximo_zombie_a_lanzar + 1) % 3;
			asd = infoJuego.zombies_disponibles[jugadorB.proximo_zombie_a_lanzar];
			print( asd, jugadorB.posicion_x, jugadorB.posicion_y, C_FG_WHITE | C_BG_BLUE  );

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

