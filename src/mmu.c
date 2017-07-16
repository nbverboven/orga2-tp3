/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

unsigned int proxima_pagina_libre;

void mmu_inicializar()
{
	proxima_pagina_libre = 0x100000;
}


void mmu_inicializar_dir_kernel()
{
	int* page_directory = (int*) PAGE_DIRECTORY_KERNEL;  // PAGE_DIRECTORY_KERNEL = 0x27000
	page_directory[0] = PAGE_TABLE_KERNEL + 0x3;		 // PAGE_TABLE_KERNEL = 0x28000, seteo p=1 y r/w=1

	// Pongo todo el resto de las posiciones en cero.
	for (int i = 1; i < 1024; ++i)
	{ 
		page_directory[i]= 0;
	}

	// Inicializo page_table
	int* page_table_1 = (int*) PAGE_TABLE_KERNEL;
	for (int i = 0; i < 1024; ++i)
	{
		page_table_1[i] = ((i << 12) | 3);
	}
}


unsigned int mmu_proxima_pagina_fisica_libre()
{
	unsigned int pagina = proxima_pagina_libre;
	proxima_pagina_libre = proxima_pagina_libre + 0x1000;
	return pagina;
}


int* mmu_inicializar_dir_zombi(unsigned int codigo_tarea, unsigned int jugador,
	                           unsigned int posicion_en_x, unsigned int posicion_en_y)
{
	int* page_directory = (int*) mmu_proxima_pagina_fisica_libre();


	/************** Identity mapping de los primeros 4Mb **************/

	int* tabla_identity_map = (int*) mmu_proxima_pagina_fisica_libre();

	// Nivel de privilegio de usuario, solo lectura, página presente
	page_directory[0] = (int) (tabla_identity_map) + 0x5;

	for ( int i = 1; i < 1024; ++i )
	{ 
		page_directory[i] = 0;
	}


	// Inicializo tabla de páginas; mismos atributos que la entrada del directorio de tablas
	for ( int i = 0; i < 1024; ++i )
	{
		tabla_identity_map[i] = ((i << 12) | 5);
	}

	/********************* Fin identity mapping ***********************/

	mmu_mapear_paginas_zombie(codigo_tarea, jugador, (unsigned int) page_directory, posicion_en_x, posicion_en_y);

	return page_directory;
}


void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica,
                       unsigned char rw, unsigned char us)
{
	unsigned int indice_pd = virtual >> 22; // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual & 0x003FF000) >> 12; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) cr3;

	// Si la tabla de páginas no existe, la creo
	if ( !( page_directory[indice_pd].present ) )
	{
		int* nueva_pt = (int*) mmu_proxima_pagina_fisica_libre();

		for ( int i = 0; i < 1024; ++i )
		{
			nueva_pt[i] = 0;
		}

		page_directory[indice_pd].present = 1;
		page_directory[indice_pd].read_write = rw;
		page_directory[indice_pd].user_supervisor = us;
		page_directory[indice_pd].page_table_address = (unsigned int) nueva_pt >> 12;
	}

	// Completo la entrada correspondiente de la tabla de páginas
	pt_entry* page_table = (pt_entry*) ( page_directory[indice_pd].page_table_address << 12 );
	page_table[indice_pt].present = 1;
	page_table[indice_pt].read_write = rw;
	page_table[indice_pt].user_supervisor = us;
	page_table[indice_pt].page_frame_address = fisica >> 12;

	tlbflush();
}


void mmu_desmapear_pagina(unsigned int virtual, unsigned int cr3)
{
	unsigned int indice_pd = virtual >> 22;                // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual & 0x003FF000) >> 12; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) cr3;
	pt_entry* page_table = (pt_entry*) ( page_directory[indice_pd].page_table_address << 12 );
	page_table[indice_pt].present = 0;

	tlbflush();
}


void mmu_desmapear_paginas_zombie(unsigned int cr3)
{
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA,        cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x1000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x2000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x3000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x4000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x5000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x6000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x7000, cr3 );
	mmu_desmapear_pagina( DIR_VIRTUAL_MAPA+0x8000, cr3 );
}


void mmu_mapear_paginas_zombie(unsigned int codigo_tarea, unsigned int jugador, unsigned int cr3,
	                           unsigned int posicion_en_x, unsigned int posicion_en_y)
{
	// Mapeo las páginas de la tarea
	map_tile (*mapa)[SIZE_W] = (map_tile (*)[SIZE_W]) INICIO_MAPA;
	
	if ( 0 < posicion_en_x && posicion_en_x < SIZE_W-1  )
	{
		unsigned int posicion_en_mapa = (unsigned int) &( mapa [posicion_en_y]                   [posicion_en_x]             );
		unsigned int adelante =         (unsigned int) &( mapa [posicion_en_y]                   [posicion_en_x+1-2*jugador] );
		unsigned int adelante_der =     (unsigned int) &( mapa [(posicion_en_y+1+42*jugador)%44] [posicion_en_x+1-2*jugador] );
		unsigned int adelante_izq =     (unsigned int) &( mapa [(posicion_en_y+43+2*jugador)%44] [posicion_en_x+1-2*jugador] );
		unsigned int der =              (unsigned int) &( mapa [(posicion_en_y+1+42*jugador)%44] [posicion_en_x]             );
		unsigned int izq =              (unsigned int) &( mapa [(posicion_en_y+43+2*jugador)%44] [posicion_en_x]             );
		unsigned int atras =            (unsigned int) &( mapa [posicion_en_y]                   [posicion_en_x-1+2*jugador] );
		unsigned int atras_izq =        (unsigned int) &( mapa [(posicion_en_y+43+2*jugador)%44] [posicion_en_x-1+2*jugador] );
		unsigned int atras_der =        (unsigned int) &( mapa [(posicion_en_y+1+42*jugador)%44] [posicion_en_x-1+2*jugador] );

		mmu_mapear_pagina( DIR_VIRTUAL_MAPA,        cr3, posicion_en_mapa, 1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x1000, cr3, adelante,         1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x2000, cr3, adelante_der,     1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x3000, cr3, adelante_izq,     1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x4000, cr3, der,              1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x5000, cr3, izq,              1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x6000, cr3, atras,            1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x7000, cr3, atras_izq,        1, 1 );
		mmu_mapear_pagina( DIR_VIRTUAL_MAPA+0x8000, cr3, atras_der,        1, 1 );

		// Copio el código de la tarea a la ubicación en el mapa
		char* src = (char*) codigo_tarea;
		char* dst = (char*) posicion_en_mapa;

		// Mapeo la página donde quiero copiar el código en el directorio de la tarea, pero con privilegio de supervisor




		/* 
		   Puede paracer una cochinada (y probablemente lo sea), pero tiene una explicación. Esta función puede ser llamada
		   desde dos posibles lugares: desde mmu_inicializar_dir_zombie o desde la rutina de atención de la syscall mover.
		   En el primer caso, la tarea activa puede ser o el kernel o la tarea idle; en ambos, el mapa de memoria que se 
		   utiliza es el del kernel, por lo que la única región mapeada son los primeros 4 MB. Lógicamente, para acceder a
		   una dirección por fuera de ese rango debe mapearse y debe utilizarse un page directory acorde, que resulta ser el
		   del kernel.
		   La otra posibilidad es que la función sea llamada al momento de querer mover un zombie. 
		*/
		mmu_mapear_pagina( posicion_en_mapa, PAGE_DIRECTORY_KERNEL, posicion_en_mapa, 1, 0 );
		mmu_mapear_pagina( posicion_en_mapa, cr3, posicion_en_mapa, 1, 0 );

		while ( (unsigned int) (src) < codigo_tarea+0x1000 )
		{
			*dst = *src;
			dst += 0x1;
			src += 0x1;
		}

		mmu_desmapear_pagina( posicion_en_mapa, PAGE_DIRECTORY_KERNEL );
		mmu_desmapear_pagina( posicion_en_mapa, cr3 );
	}
}