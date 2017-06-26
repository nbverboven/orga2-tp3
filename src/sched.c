/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

unsigned short sched_proximo_indice()
{
	return 0;
}

void sched_handler_teclado(unsigned int tecla)
{
	const char* asd;
	unsigned short attr;

	switch ( tecla )
	{
		/********** Teclas para el jugador A ************/

		// W
		case 0x11:
			asd = (const char*) "W";
			attr = ( C_FG_LIGHT_CYAN | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0x91:
			asd = (const char*) "W";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		// A
		case 0x1E:
			asd = (const char*) "A";
			attr = ( C_FG_LIGHT_CYAN | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0x9E:
			asd = (const char*) "A";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		// S
		case 0x1F:
			asd = (const char*) "Puto el que lee";
			attr = ( C_FG_LIGHT_CYAN | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0x9F:
			asd = (const char*) "Puto el que lee";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		// D
		case 0x20:
			asd = (const char*) "D";
			attr = ( C_FG_LIGHT_CYAN | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0xA0:
			asd = (const char*) "D";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
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
			asd = (const char*) "I";
			attr = ( C_FG_LIGHT_MAGENTA | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		case 0x97:
			asd = (const char*) "I";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		// K
		case 0x25:
			asd = (const char*) "K";
			attr = ( C_FG_LIGHT_MAGENTA | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		case 0xA5:
			asd = (const char*) "K";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		// L
		case 0x26:
			asd = (const char*) "Gil";
			attr = ( C_FG_LIGHT_MAGENTA | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		case 0xA6:
			asd = (const char*) "Gil";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		// J
		case 0x24:
			asd = (const char*) "J";
			attr = ( C_FG_LIGHT_MAGENTA | C_BG_BLACK );
			print( asd, 18, 0, attr );
			break;

		case 0xA4:
			asd = (const char*) "J";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 18, 0, attr );
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

		// H
		case 0x23:
			asd = (const char*) "H";
			attr = ( C_FG_LIGHT_BROWN | C_BG_BLACK );
			print( asd, 25, 0, attr );
			break;

		case 0xA3:
			asd = (const char*) "H";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 25, 0, attr );
			break;

	}
}

