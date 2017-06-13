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




