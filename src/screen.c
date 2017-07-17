/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

unsigned char cositos[4] = {'|', '/', '-', '\\'};

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr)
{
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
	int i;
	for (i = 0; text[i] != 0; i++)
	{
		p[y][x].c = (unsigned char) text[i];
		p[y][x].a = (unsigned char) attr;
		x++;
		if (x == VIDEO_COLS)
		{
			x = 0;
			y++;
		}
	}
}


void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr)
{
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
	int i;
	char hexa[8];
	char letras[16] = "0123456789ABCDEF";
	hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
	hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
	hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
	hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
	hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
	hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
	hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
	hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
	for(i = 0; i < size; i++)
	{
		p[y][x + size - i - 1].c = hexa[i];
		p[y][x + size - i - 1].a = attr;
	}
}


void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr)
{
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
	if( n > 9 )
	{
	  int a = n / 10;
	  n -= 10 * a;
	  print_int(a,x-1,y,attr);
	}
	p[y][x].c = '0'+n;
	p[y][x].a = attr;
}


void actualizar_info_pantalla()
{
	// Imprimo los puntajes
	print_int(infoJuego.jugador_A.puntaje_actual, 37, 47, C_FG_WHITE | C_BG_RED); // A
	print_int(infoJuego.jugador_B.puntaje_actual, 43, 47, C_FG_WHITE | C_BG_BLUE); // B

	// Hago esto porque si no, por alguna razón que no me puse a pensar, pasa de escribir
	// 10 a escribir 19 cuando le resto un zombie. Lo mismo vale para lo que está más abajo
	if ( infoJuego.jugador_A.puntaje_actual < 10 )
	{
		print_int(0, 36, 47, C_FG_WHITE | C_BG_RED);
	}

	if ( infoJuego.jugador_B.puntaje_actual < 10 )
	{
		print_int(0, 42, 47, C_FG_WHITE | C_BG_BLUE);
	}

	// Imprimo los zombies que le quedan a cada jugador
	print_int(infoJuego.jugador_A.zombies_restantes, 31, 47, C_FG_WHITE | C_BG_RED); // A
	print_int(infoJuego.jugador_B.zombies_restantes, 49, 47, C_FG_WHITE | C_BG_BLUE); // B

	if ( infoJuego.jugador_A.zombies_restantes < 10 )
	{
		print_int(0, 30, 47, C_FG_WHITE | C_BG_RED);
	}

	if ( infoJuego.jugador_B.zombies_restantes < 10 )
	{
		print_int(0, 48, 47, C_FG_WHITE | C_BG_BLUE);
	}

	actualizar_relojes();
}


void actualizar_relojes()
{
	// Actualizo los relojes de las tareas del jugador A
	int i = 0;
	unsigned int x_a = 5;
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

	while ( i < CANT_ZOMBIS )
	{
		if ( infoJuego.tareasA[i].esta_activa )
		{
			p[48][x_a].c = cositos[infoJuego.tareasA[i].z_reloj];
			p[48][x_a].a = C_FG_WHITE | C_BG_BLACK;
			infoJuego.tareasA[i].z_reloj = ( infoJuego.tareasA[i].z_reloj + 1 ) %4;
		}
		else
		{
			p[48][x_a].c = 'X';
			p[48][x_a].a = C_FG_RED | C_BG_BLACK;
		}
		
		++i;
		x_a += 2;
	}

	// Actualizo los relojes de las tareas del jugador B
	int j = 0;
	unsigned int x_b= 60;

	while ( j < CANT_ZOMBIS )
	{
		if ( infoJuego.tareasB[j].esta_activa )
		{
			p[48][x_b].c = cositos[infoJuego.tareasB[j].z_reloj];
			p[48][x_b].a = C_FG_WHITE | C_BG_BLACK;
			infoJuego.tareasB[j].z_reloj = ( infoJuego.tareasB[j].z_reloj + 1 ) %4;
		}
		else
		{
			p[48][x_b].c = 'X';
			p[48][x_b].a = C_FG_BLUE | C_BG_BLACK;
		}
		
		++j;
		x_b += 2;
	}
}



