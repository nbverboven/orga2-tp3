/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

unsigned char cositos[4] = {'|', '/', '-', '\\'};

// Estructura auxiliar para almacenar la información de los registros
// cuando se produce una excepción
struct {
	unsigned int   eax;
	unsigned int   ebx;
	unsigned int   ecx;
	unsigned int   edx;
	unsigned int   esi;
	unsigned int   edi;
	unsigned int   ebp;
	unsigned int   esp;
	unsigned int   eip;
	unsigned short cs;
	unsigned short ds;
	unsigned short es;
	unsigned short fs;
	unsigned short gs;
	unsigned short ss;
	unsigned int   eflags;
	unsigned int   cr0;
	unsigned int   cr2;
	unsigned int   cr3;
	unsigned int   cr4;
	unsigned int   stack1;
	unsigned int   stack2;
	unsigned int   stack3;
	unsigned int   stack4;
	unsigned int   stack5;
} __attribute__((__packed__)) info_debug;

// Estructura para almacenar la información de la pantalla 
// al imprimir la pantalla de debug
struct {
	ca buffer[VIDEO_COLS*VIDEO_FILS];
} __attribute__((__packed__)) info_pantalla;

unsigned int pagina_aux;


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

	// Hago esto porque, si no, el puntaje que se muestra en pantalla es, por poner un
	// ejemplo, 5 en lugar de 05. Solo se aplica para valores desde 0 hasta 9.
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

	// Hago esto porque, si no, pasa de escribir 10 a escribir 19 cuando le resto un zombie. 
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


void imprimir_pantalla_debug()
{
	// Guardo la pantalla actual 
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
	ca (*buffer_pantalla)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) &info_pantalla.buffer;

	for (int i = 1; i < 45; ++i)
	{
		for (int j = 1; j < 79; ++j)
		{
			buffer_pantalla[i][j].c = p[i][j].c;
			buffer_pantalla[i][j].a = p[i][j].a;
		}
	}

	// Pinto la pantalla de debug
	for (int i = 7; i < 43; ++i)
	{
		for (int j = 25; j < 55; ++j)
		{
			print( "a", j, i, C_FG_BLACK | C_BG_BLACK );
		}
	}

	for (int i = 8; i < 42; ++i)
	{
		for (int j = 26; j < 54; ++j)
		{
			print( "a", j, i, C_FG_LIGHT_GREY | C_BG_LIGHT_GREY );
		}
	}

	for (int i = 26; i < 54; ++i)
	{
		print( "a", i, 8, C_FG_BLUE | C_BG_BLUE );
	}

	const char* columna_1[16] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp",
	                             "eip", "cs", "ds", "es", "fs", "gs", "ss", "eflags"};

	const char* screen_zombis[3] = {"Guerrero", "Mago", "Clerigo"};

	print("Zombie", 26, 8, C_FG_WHITE | C_BG_BLUE);

	// Imprimo qué tipo de zombie murió y a quién le pertenecía
	if ( !infoJuego.jugador_de_turno ) 
	{
		print("A", 33, 8, C_FG_WHITE | C_BG_BLUE);
		print(screen_zombis[infoJuego.tareasA[infoJuego.tarea_actual_A].z_tipo], 35, 8, C_FG_WHITE | C_BG_BLUE);
	}
	else
	{
		print("B", 33, 8, C_FG_WHITE | C_BG_BLUE);
		print(screen_zombis[infoJuego.tareasB[infoJuego.tarea_actual_B].z_tipo], 35, 8, C_FG_WHITE | C_BG_BLUE);
	}

	// Imprimo los elementos de columna_1 en su lugar correcto en la pantalla
	int k = 10;

	for (int i = 0; i < 16; ++i)
	{
		print( columna_1[i], 27, k, C_FG_BLACK | C_BG_LIGHT_GREY );
		k += 2;
	}

	print( "cr0", 41, 10, C_FG_BLACK | C_BG_LIGHT_GREY );
	print( "cr2", 41, 12, C_FG_BLACK | C_BG_LIGHT_GREY );
	print( "cr3", 41, 14, C_FG_BLACK | C_BG_LIGHT_GREY );
	print( "cr4", 41, 16, C_FG_BLACK | C_BG_LIGHT_GREY );
	print( "stack", 41, 27, C_FG_BLACK | C_BG_LIGHT_GREY );

	// Imprimo la información de debug contenida en la estructura del mismo nombre
	print_hex( info_debug.stack1, 8, 41, 29, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.stack2, 8, 41, 30, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.stack3, 8, 41, 31, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.stack4, 8, 41, 32, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.stack5, 8, 41, 33, C_FG_WHITE | C_BG_LIGHT_GREY );

	print_hex( info_debug.eax, 8, 31, 10, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.ebx, 8, 31, 12, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.ecx, 8, 31, 14, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.edx, 8, 31, 16, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.esi, 8, 31, 18, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.edi, 8, 31, 20, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.ebp, 8, 31, 22, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.esp, 8, 31, 24, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.eip, 8, 31, 26, C_FG_WHITE | C_BG_LIGHT_GREY );

	print_hex( info_debug.cs, 4, 31, 28, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.ds, 4, 31, 30, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.es, 4, 31, 32, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.fs, 4, 31, 34, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.gs, 4, 31, 36, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.ss, 4, 31, 38, C_FG_WHITE | C_BG_LIGHT_GREY );

	print_hex( info_debug.eflags, 8, 34, 40, C_FG_WHITE | C_BG_LIGHT_GREY );

	print_hex( info_debug.cr0, 8, 45, 10, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.cr2, 8, 45, 12, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.cr3, 8, 45, 14, C_FG_WHITE | C_BG_LIGHT_GREY );
	print_hex( info_debug.cr4, 8, 45, 16, C_FG_WHITE | C_BG_LIGHT_GREY );
}


void restaurar_pantalla()
{
	// Vuelvo a pintar la pantalla con la info contenida en el buffer
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
	ca (*buffer_pantalla)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) &info_pantalla.buffer;

	for (int i = 1; i < 45; ++i)
	{
		for (int j = 1; j < 79; ++j)
		{
			p[i][j].c = buffer_pantalla[i][j].c;
			p[i][j].a = buffer_pantalla[i][j].a;
		}
	}
}