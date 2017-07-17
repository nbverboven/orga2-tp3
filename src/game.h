/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "mmu.h"
#include "tss.h"
#include "sched.h"
#include "screen.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(unsigned int value);
void game_lanzar_zombi(unsigned int jugador);
void game_move_current_zombi(direccion dir);
unsigned int game_dame_codigo(zombie_type tipo, unsigned char j);

#endif  /* !__GAME_H__ */
