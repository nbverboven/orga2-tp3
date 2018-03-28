/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"

void mmu_inicializar();
void mmu_inicializar_dir_kernel();
unsigned int mmu_proxima_pagina_fisica_libre();
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char rw, unsigned char us);
void mmu_desmapear_pagina(unsigned int virtual, unsigned int cr3);
void mmu_desmapear_paginas_zombie(unsigned int cr3);
int* mmu_inicializar_dir_zombi(unsigned int codigo_tarea, unsigned int jugador,
	                           unsigned int posicion_en_x, unsigned int posicion_en_y);
void mmu_mapear_paginas_zombie(unsigned int codigo_tarea, unsigned int jugador, unsigned int cr3,
	                           unsigned int posicion_en_x, unsigned int posicion_en_y);


typedef struct str_pd_entry {
	unsigned char present:1;
	unsigned char read_write:1;                 // 1 = readable/writable; 0 = read only
	unsigned char user_supervisor:1;            // 1 = user; 0 = supervisor
	unsigned char page_level_write_through:1;
	unsigned char page_level_cache_disabled:1;
	unsigned char accesed:1;
	unsigned char ignored_1:1;
	unsigned char page_size:1;                  // 0 = 4Kb, 1 = 4Mb
	unsigned char ignored_2:4;
	unsigned int page_table_address:20;         // Dirección física de la tabla de página
} __attribute__((__packed__, aligned (4))) pd_entry;


typedef struct str_pt_entry {
	unsigned char present:1;
	unsigned char read_write:1;                 // 1 = readable/writable; 0 = read only
	unsigned char user_supervisor:1;            // 1 = user; 0 = supervisor
	unsigned char page_level_write_through:1;
	unsigned char page_level_cache_disabled:1;
	unsigned char accesed:1;
	unsigned char dirty:1;                      // Si la página fue modificada, se setea en 1
	unsigned char page_attribute_table:1;       // (?)
	unsigned char global:1;                     // (?)
	unsigned char ignored:3;
	unsigned int page_frame_address:20;         // Dirección física del frame de una página de 4Kb
} __attribute__((__packed__, aligned (4))) pt_entry;


// Una estructura auxiliar que ocupa 4096 bytes (4 Kb)
typedef struct str_map_tile {
	double aux[512]; 
} __attribute__((__packed__)) map_tile;


// typedef struct str_dir_virtual {
// 	unsigned short offset:12;
// 	unsigned short page_table_index:10;
// 	unsigned short page_directory_index:10;
// } __attribute__((__packed__)) dir_virtual;



//extern pd_entry page_directory[];
//extern pt_entry page_table[];

#endif	/* !__MMU_H__ */




