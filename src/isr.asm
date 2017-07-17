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

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
	pushad

	mov eax, %1
	imprimir_texto_mp  msg_int_%1, len_int_%1, 0x07, 0, 0

	call sched_desalojar_tarea_actual

	mov ax, 0x0070 ; 0x0070 = 0000 0000 0111 0000. índice = 0000000001110 (14)  gdt/ldt = 0  dpl = 00 
	mov [sched_tarea_selector], ax ; Cargo el selector de tss de la tarea idle
	jmp far [sched_tarea_offset]

	popad
	iret

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'
estoy_en_modo_debug: db 0


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
	call actualizar_info_pantalla
	call sched_proximo_indice

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

	xor eax, eax
	in al, 0x60
	
	push eax
	call game_jugador_mover
	add esp, 4

	call fin_intr_pic1

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
	; call sched_ejecutar_orden_66
	call game_move_current_zombi
	add esp, 4

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
		
		
