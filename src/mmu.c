/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

unsigned int proxima_pagina_libre;

void mmu_inicializar() {
	proxima_pagina_libre = 0x29000;
}

void mmu_inicializar_dir_kernel() {

	// Inicializo directorio de paginas en pos 0x27000
	int* page_directory = (int*) 0x27000;  // PAGE_DIRECTORY_KERNEL = 0x27000
	// Seteo directorio de tablas en pos 0x28000
	page_directory[0] = 0x28000 + 0x3;		 // PAGE_TABLE_KERNEL = 0x28000, seteo p=1 y r/w=1

	// Pongo todo el resto de las posiciones en cero.
	for (int i = 1; i < 1024; ++i) { 
	page_directory[i]= 0x0;
	}

	// Inicializo page_directory 
	int* page_table_1 = (int*) 0x28000;
	for (int i = 0; i < 1024; ++i) {
		page_table_1[i] = ((i << 12) | 3);
	}

}


unsigned int mmu_proxima_pagina_fisica_libre(){
	unsigned int pagina = proxima_pagina_libre;
	proxima_pagina_libre = proxima_pagina_libre + 0x1000;
	return pagina;
}


int* mmu_inicializar_dir_zombi(unsigned int id_tarea, unsigned int codigo_tarea){//TODO ver parametros necesarios
	int* page_directory = (int*) mmu_proxima_pagina_fisica_libre();  
	// Inicializo en 0x0
	for (int i = 1; i < 1024; ++i) {
	  page_directory[i] = 0x0;
	}

	int* page_table_1 = (int*) mmu_proxima_pagina_fisica_libre();
	for (int i = 0; i < 1024; ++i) {
		page_table_1[i] = ((i << 12) | 7); //user,read only,present
	}

	// Seteo la pagina_1 en el page directory del dir_zombi
	page_directory[0] = (int)page_table_1 + 0x7;



	//Setear mapa en el page directory

	return page_directory;


}


void mmu_mapear_pagina(unsigned int virtual, unsigned int dir_pd, unsigned int fisica, 
	                   unsigned char rw, unsigned char us)
{
	unsigned int indice_pd = (virtual & 0xFFC00000) >> 22; // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual & 0x003FF000) >> 12; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) dir_pd;

	// Si la tabla de páginas no existe, la creo
	if ( !(page_directory[indice_pd].present) )
	{
		int* nueva_pt = (int*) mmu_proxima_pagina_fisica_libre();

		for (int i = 0; i < 1024; ++i)
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


void mmu_desmapear_pagina(unsigned int virtual, unsigned int dir_pd)
{
	unsigned int indice_pd = (virtual & 0xFFC00000) >> 22; // Me quedo con los bits 22-31
	unsigned int indice_pt = (virtual & 0x003FF000) >> 12; // Me quedo con los bits 12-21

	pd_entry* page_directory = (pd_entry*) dir_pd;

	if ( page_directory[indice_pd].present )
	{
		pt_entry* page_table = (pt_entry*) ( page_directory[indice_pd].page_table_address << 12 );
		page_table[indice_pt].present = 0;
	}

	tlbflush();
}