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
		case 0x1E:
			asd = (const char*) "Puto el que lee";
			attr = ( C_FG_WHITE | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;

		case 0x9E:
			asd = (const char*) "Puto el que lee";
			attr = ( C_FG_BLACK | C_BG_BLACK );
			print( asd, 0, 0, attr );
			break;
	}
}

