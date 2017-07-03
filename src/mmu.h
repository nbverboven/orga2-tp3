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
#include "game.h"

void mmu_inicializar();
void mmu_inicializar_dir_kernel();
unsigned int mmu_proxima_pagina_fisica_libre();
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char rw, unsigned char us);
// unsigned int mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica, unsigned char rw, unsigned char us);
void mmu_desmapear_pagina(unsigned int virtual, unsigned int cr3);
int* mmu_inicializar_dir_zombi(unsigned int codigo_tarea, unsigned int jugador,
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
	double a1; double a2; double a3; double a4; double a5; double a6; double a7; double a8; double a9; double a10; double a11; double a12; double a13; double a14; double a15; double a16;
	double b1; double b2; double b3; double b4; double b5; double b6; double b7; double b8; double b9; double b10; double b11; double b12; double b13; double b14; double b15; double b16;
	double c1; double c2; double c3; double c4; double c5; double c6; double c7; double c8; double c9; double c10; double c11; double c12; double c13; double c14; double c15; double c16;
	double d1; double d2; double d3; double d4; double d5; double d6; double d7; double d8; double d9; double d10; double d11; double d12; double d13; double d14; double d15; double d16;
	double e1; double e2; double e3; double e4; double e5; double e6; double e7; double e8; double e9; double e10; double e11; double e12; double e13; double e14; double e15; double e16;
	double f1; double f2; double f3; double f4; double f5; double f6; double f7; double f8; double f9; double f10; double f11; double f12; double f13; double f14; double f15; double f16;
	double g1; double g2; double g3; double g4; double g5; double g6; double g7; double g8; double g9; double g10; double g11; double g12; double g13; double g14; double g15; double g16;
	double h1; double h2; double h3; double h4; double h5; double h6; double h7; double h8; double h9; double h10; double h11; double h12; double h13; double h14; double h15; double h16;

	double a17; double a18; double a19; double a20; double a21; double a22; double a23; double a24; double a25; double a26; double a27; double a28; double a29; double a30; double a31; double a32;
	double b17; double b18; double b19; double b20; double b21; double b22; double b23; double b24; double b25; double b26; double b27; double b28; double b29; double b30; double b31; double b32;
	double c17; double c18; double c19; double c20; double c21; double c22; double c23; double c24; double c25; double c26; double c27; double c28; double c29; double c30; double c31; double c32;
	double d17; double d18; double d19; double d20; double d21; double d22; double d23; double d24; double d25; double d26; double d27; double d28; double d29; double d30; double d31; double d32;
	double e17; double e18; double e19; double e20; double e21; double e22; double e23; double e24; double e25; double e26; double e27; double e28; double e29; double e30; double e31; double e32;
	double f17; double f18; double f19; double f20; double f21; double f22; double f23; double f24; double f25; double f26; double f27; double f28; double f29; double f30; double f31; double f32;
	double g17; double g18; double g19; double g20; double g21; double g22; double g23; double g24; double g25; double g26; double g27; double g28; double g29; double g30; double g31; double g32;
	double h17; double h18; double h19; double h20; double h21; double h22; double h23; double h24; double h25; double h26; double h27; double h28; double h29; double h30; double h31; double h32;

	double a33; double a34; double a35; double a36; double a37; double a38; double a39; double a40; double a41; double a42; double a43; double a44; double a45; double a46; double a47; double a48;
	double b33; double b34; double b35; double b36; double b37; double b38; double b39; double b40; double b41; double b42; double b43; double b44; double b45; double b46; double b47; double b48;
	double c33; double c34; double c35; double c36; double c37; double c38; double c39; double c40; double c41; double c42; double c43; double c44; double c45; double c46; double c47; double c48;
	double d33; double d34; double d35; double d36; double d37; double d38; double d39; double d40; double d41; double d42; double d43; double d44; double d45; double d46; double d47; double d48;
	double e33; double e34; double e35; double e36; double e37; double e38; double e39; double e40; double e41; double e42; double e43; double e44; double e45; double e46; double e47; double e48;
	double f33; double f34; double f35; double f36; double f37; double f38; double f39; double f40; double f41; double f42; double f43; double f44; double f45; double f46; double f47; double f48;
	double g33; double g34; double g35; double g36; double g37; double g38; double g39; double g40; double g41; double g42; double g43; double g44; double g45; double g46; double g47; double g48;
	double h33; double h34; double h35; double h36; double h37; double h38; double h39; double h40; double h41; double h42; double h43; double h44; double h45; double h46; double h47; double h48;

	double a49; double a50; double a51; double a52; double a53; double a54; double a55; double a56; double a57; double a58; double a59; double a60; double a61; double a62; double a63; double a64;
	double b49; double b50; double b51; double b52; double b53; double b54; double b55; double b56; double b57; double b58; double b59; double b60; double b61; double b62; double b63; double b64;
	double c49; double c50; double c51; double c52; double c53; double c54; double c55; double c56; double c57; double c58; double c59; double c60; double c61; double c62; double c63; double c64;
	double d49; double d50; double d51; double d52; double d53; double d54; double d55; double d56; double d57; double d58; double d59; double d60; double d61; double d62; double d63; double d64;
	double e49; double e50; double e51; double e52; double e53; double e54; double e55; double e56; double e57; double e58; double e59; double e60; double e61; double e62; double e63; double e64;
	double f49; double f50; double f51; double f52; double f53; double f54; double f55; double f56; double f57; double f58; double f59; double f60; double f61; double f62; double f63; double f64;
	double g49; double g50; double g51; double g52; double g53; double g54; double g55; double g56; double g57; double g58; double g59; double g60; double g61; double g62; double g63; double g64;
	double h49; double h50; double h51; double h52; double h53; double h54; double h55; double h56; double h57; double h58; double h59; double h60; double h61; double h62; double h63; double h64;
} __attribute__((__packed__)) map_tile;


// typedef struct str_dir_virtual {
// 	unsigned short offset:12;
// 	unsigned short page_table_index:10;
// 	unsigned short page_directory_index:10;
// } __attribute__((__packed__)) dir_virtual;



//extern pd_entry page_directory[];
//extern pt_entry page_table[];

#endif	/* !__MMU_H__ */




