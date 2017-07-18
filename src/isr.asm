; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

msg_int_0 db 'Devide error wacho!'
len_int_0 equ $ - msg_int_0

msg_int_1 db 'RESERVED vieja!'
len_int_1 equ $ - msg_int_1

msg_int_2 db 'NMI interrupt!'
len_int_2 equ $ - msg_int_2

msg_int_3 db 'Breakpoint wachin!'
len_int_3 equ $ - msg_int_3

msg_int_4 db 'Overflow, rescatate!'
len_int_4 equ $ - msg_int_4

msg_int_5 db 'BOUND Range Exceeded, manejate!'
len_int_5 equ $ - msg_int_5

msg_int_6 db 'Invalid Opcode (Undefined Opcode), hiciste cualquiera!'
len_int_6 equ $ - msg_int_6

msg_int_7 db 'Device Not Available (No Math Coprocessor), tah re loko!'
len_int_7 equ $ - msg_int_7

msg_int_8 db 'Double Fault, jodete!'
len_int_8 equ $ - msg_int_8

msg_int_9 db 'Coprocessor Segment Overrun (reserved)...!'
len_int_9 equ $ - msg_int_9

msg_int_10 db 'Invalid TSS, vo fijateh!'
len_int_10 equ $ - msg_int_10

msg_int_11 db 'Segment Not Present, anda a buscarlo!'
len_int_11 equ $ - msg_int_11

msg_int_12 db 'Stack-Segment Fault, cosas que pasan!'
len_int_12 equ $ - msg_int_12

msg_int_13 db  'General protection fault!!!!!!, gil'
len_int_13 equ $ -  msg_int_13

msg_int_14 db 'Page Fault, arreglalo!'
len_int_14 equ $ - msg_int_14

msg_int_16 db 'x87 FPU Floating-Point Error (Math Fault), mira bien lo que haces!'
len_int_16 equ $ - msg_int_16

msg_int_17 db 'Alignment Check! :-)'
len_int_17 equ $ - msg_int_17

msg_int_18 db 'Machine Check! ;)'
len_int_18 equ $ - msg_int_18

msg_int_19 db 'SIMD Floating-Point Exception! :('
len_int_19 equ $ - msg_int_19

msg_int_Y db 'Modo Debug On'
len_msg_Y equ $ - msg_int_Y

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
extern sched_handler_teclado
extern sched_ejecutar_orden_66
extern sched_desalojar_tarea_actual

;; Game 
extern game_move_current_zombi
extern game_jugador_mover

;; Screen
extern actualizar_info_pantalla
extern restaurar_pantalla
extern imprimir_pantalla_debug
extern info_debug

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
	cmp byte [estoy_en_modo_debug], 1
	jne .solo_desalojo

	mov dword [info_debug + 0], eax
	mov dword [info_debug + 4], ebx
	mov dword [info_debug + 8], ecx
	mov dword [info_debug + 12], edx
	mov dword [info_debug + 16], esi
	mov dword [info_debug + 20], edi
	mov dword [info_debug + 24], ebp
	mov dword [info_debug + 28], esp
	mov eax, [esp + 4]
	mov dword [info_debug + 32], eax
	mov word [info_debug + 36], cs
	mov word [info_debug + 38], ds
	mov word [info_debug + 40], es
	mov word [info_debug + 42], fs
	mov word [info_debug + 44], gs
	mov word [info_debug + 46], ss
	pushfd
	pop eax
	mov dword [info_debug + 48], eax
	mov eax, cr0
	mov dword [info_debug + 52], eax
	mov eax, cr2
	mov dword [info_debug + 56], eax
	mov eax, cr3
	mov dword [info_debug + 60], eax
	mov eax, cr4
	mov dword [info_debug + 64], eax
	mov eax, [esp + 4]
	mov dword [info_debug + 68], eax
	mov eax, [esp + 8]
	mov dword [info_debug + 72], eax
	mov eax, [esp + 12]
	mov dword [info_debug + 76], eax
	mov eax, [esp + 16]
	mov dword [info_debug + 80], eax
	mov eax, [esp + 20]
	mov dword [info_debug + 84], eax

	.solo_desalojo:
		call sched_desalojar_tarea_actual

		cmp byte [estoy_en_modo_debug], 0
		je .fin

		cmp byte [mostrando_pantalla_debug], 0
		jne .fin

		imprimir_texto_mp  msg_int_Y, len_msg_Y, 0x07, 0, 0
		mov byte [mostrando_pantalla_debug], 1
		call imprimir_pantalla_debug

	.fin:
		mov ax, 0x0070 ; 0x0070 = 0000 0000 0111 0000. índice = 0000000001110 (14)  gdt/ldt = 0  dpl = 00 
		mov [sched_tarea_selector], ax ; Cargo el selector de tss de la tarea idle
		jmp far [sched_tarea_offset]
		iret
%endmacro


;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:              dd 0x00000000
isrClock:               db '|/-\'
estoy_en_modo_debug:    db 0
mostrando_pantalla_debug: db 0


;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
	pushad
	pushfd

	call fin_intr_pic1
	call proximo_reloj

	cmp byte [mostrando_pantalla_debug], 1
	je .salto_a_la_idle

	call actualizar_info_pantalla
	call sched_proximo_indice
	jmp .salto_al_proximo_indice

	.salto_a_la_idle:
		mov ax, 0x0070

	.salto_al_proximo_indice:
		cmp ax, 0
		je .fin

		cmp ax, [sched_tarea_selector]
		je .fin

		mov [sched_tarea_selector], ax
		jmp far [sched_tarea_offset]

	.fin:
		popfd
		popad
		iret


;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
	pushad
	pushfd

	call fin_intr_pic1
	xor eax, eax
	in al, 0x60
	cmp al, 0x15
	je .atiendoY

	cmp byte [mostrando_pantalla_debug], 1 ; Los jugadores no pueden hacer nada mientras muestro
	je .fin                                ; la info de debug

	push eax
	call game_jugador_mover
	add esp, 4
	jmp .fin

	.atiendoY:
		cmp byte [estoy_en_modo_debug], 1
		jne .activo_modo_debug

		cmp byte [mostrando_pantalla_debug], 1
		jne .no_mostraba_nada

		; call restaurar_pantalla
		imprimir_texto_mp  msg_int_Y, len_msg_Y, 0x00, 0, 0
		mov byte [mostrando_pantalla_debug], 0
		jmp .fin

		.no_mostraba_nada:
			imprimir_texto_mp  msg_int_Y, len_msg_Y, 0x00, 0, 35
			mov byte [estoy_en_modo_debug], 0
			jmp .fin

		.activo_modo_debug:
			imprimir_texto_mp  msg_int_Y, len_msg_Y, 0x07, 0, 35
			mov byte [estoy_en_modo_debug], 1

	.fin:
		popfd
		popad
		iret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732

global _isr102 ; int 0x66
_isr102:
	pushad
	pushfd
	
	push eax
	call game_move_current_zombi
	add esp, 4

	.fin:
	mov ax, 0x0070 ; 0x0070 = 0000 0000 0111 0000. índice = 0000000001110 (14)  gdt/ldt = 0  dpl = 00 
	mov [sched_tarea_selector], ax ; Cargo el selector de tss de la tarea idle
	jmp far [sched_tarea_offset]

		popfd
		popad
	iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
	pushad
	inc DWORD [isrnumero]
	mov ebx, [isrnumero]
	cmp ebx, 0x4
	jl .ok
		mov DWORD [isrnumero], 0x0
		mov ebx, 0
	.ok:
		add ebx, isrClock
		imprimir_texto_mp ebx, 1, 0x0f, 49, 79
		popad
	ret
