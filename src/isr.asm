; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

extern is_mode_debug_on
extern print_hex

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

; pintar_pantalla_msg db ''

; msg_debug_hex db '0123456789ABCDEF'
; msg_debug_eax db 'eax'
; msg_debug_ebx db 'ebx'
; msg_debug_ecx db 'ecx'
; msg_debug_edx db 'edx'
; msg_debug_esi db 'esi'
; msg_debug_edi db 'edi'
; msg_debug_ebp db 'ebp'
; msg_debug_esp db 'esp'
; msg_debug_eip db 'eip'
; msg_debug_cs db 'cs'
; msg_debug_ds db 'ds'
; msg_debug_es db 'es'
; msg_debug_fs db 'fs'
; msg_debug_gs db 'gs'
; msg_debug_ss db 'ss'
; msg_debug_eflags db 'eflags'
; msg_debug_cr0 db 'cr0'
; msg_debug_cr2 db 'cr2'
; msg_debug_cr3 db 'cr3'
; msg_debug_cr4 db 'cr4'
; msg_debug_stack db 'stack'

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
extern restaurar_pantalla
extern imprimir_pantalla_debug
extern info_debug

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
	; pushad

	; mov eax, %1
	imprimir_texto_mp  msg_int_%1, len_int_%1, 0x07, 0, 0

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

	call imprimir_pantalla_debug

; Esto está para ver como queda. Después se comenta o se saca
jmp $

	call sched_desalojar_tarea_actual

	mov ax, 0x0070 ; 0x0070 = 0000 0000 0111 0000. índice = 0000000001110 (14)  gdt/ldt = 0  dpl = 00 
	mov [sched_tarea_selector], ax ; Cargo el selector de tss de la tarea idle
	jmp far [sched_tarea_offset]

	; popad
	iret

%endmacro


; %macro imprimir_hexa 4
; 	pushad
; 	pushfd
    
;     mov eax, %1 ; %1 = registro
; 	mov esi, %2 ; %2 = longitud
;     mov ecx, %3	; %3 = fila
; 	mov edx, %4	; %4 = columna
	
; 	add esi, edx

; 	%%impimo:
; 		mov ebx, eax
; 		mov bl, bl
; 		add ebx, msg_debug_hex
; 		imprimir_texto_mp ebx, 1, 0x7F, ecx, edx
; 		inc edx
; 		shr eax, 8

; 		cmp edx, esi
; 		jne %%impimo

; 	popfd
; 	popad
; %endmacro

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
	; call is_mode_debug_on
	; cmp ax, 0
	; je .debug_off
	; 	call ventana_debug
	; 	jmp .fin

	; .debug_off:
		call game_move_current_zombi
		;call sched_ejecutar_orden_66
		add esp, 4

		mov ax, 0x0070 ; 0x0070 = 0000 0000 0111 0000. índice = 0000000001110 (14)  gdt/ldt = 0  dpl = 00 
		mov [sched_tarea_selector], ax ; Cargo el selector de tss de la tarea idle
		jmp far [sched_tarea_offset]

	.fin:
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





;; Ventada Debug
;; -------------------------------------------------------------------------- ;;
; ventana_debug:
; 	;50*36 

; 	imprimir_texto_mp pintar_pantalla_msg, 30, 0x00, 7, 24
; 	imprimir_texto_mp pintar_pantalla_msg, 1, 0x00, 8, 24
; 	imprimir_texto_mp pintar_pantalla_msg, 28, 00010001b, 8, 25
; 	imprimir_texto_mp pintar_pantalla_msg, 1, 0x00, 8, 53

; 	mov ecx, 34
; 	mov ecx, ecx

; 	.background_ventana:
; 		mov eax, ecx
; 		add eax, 8
; 		imprimir_texto_mp pintar_pantalla_msg, 30, 0x00, eax, 24
; 		imprimir_texto_mp pintar_pantalla_msg, 28, 0x77, eax, 25
; 	loop .background_ventana

; 	imprimir_texto_mp pintar_pantalla_msg, 30, 0x00, 42, 24


; 	;; --- IMPRIMO LOS REGISTROS ---
; 	;popfd
; 	;popad

; 	;xchg bx,bx ;breakpoint

; 	imprimir_texto_mp msg_debug_eax, 3, 0x70, 10, 26
; 	imprimir_hexa eax, 8, 10, 30

; 	imprimir_texto_mp msg_debug_ebx, 3, 0x70, 12, 26
; 	imprimir_hexa ebx, 8, 12, 30

; 	imprimir_texto_mp msg_debug_ecx, 3, 0x70, 14, 26
; 	imprimir_hexa ecx, 8, 14, 30

; 	imprimir_texto_mp msg_debug_edx, 3, 0x70, 16, 26
; 	imprimir_hexa edx, 8, 16, 30

; 	imprimir_texto_mp msg_debug_esi, 3, 0x70, 18, 26
; 	imprimir_hexa esi, 8, 18, 30

; 	imprimir_texto_mp msg_debug_edi, 3, 0x70, 20, 26
; 	imprimir_hexa edi, 8, 20, 30

; 	imprimir_texto_mp msg_debug_ebp, 3, 0x70, 22, 26
; 	imprimir_hexa ebp, 8, 22, 30

; 	imprimir_texto_mp msg_debug_esp, 3, 0x70, 24, 26
; 	imprimir_hexa esp, 8, 24, 30

; 	imprimir_texto_mp msg_debug_eip, 3, 0x70, 26, 26
; 	imprimir_hexa [esp+4], 8, 26, 30
	
; 	imprimir_texto_mp msg_debug_cs, 2, 0x70, 28, 27
; 	imprimir_hexa cs, 4, 28, 30

; 	imprimir_texto_mp msg_debug_ds, 2, 0x70, 30, 27
; 	imprimir_hexa ds, 4, 32, 30

; 	imprimir_texto_mp msg_debug_es, 2, 0x70, 32, 27
; 	imprimir_hexa es, 4, 32, 30

; 	imprimir_texto_mp msg_debug_fs, 2, 0x70, 34, 27
; 	imprimir_hexa fs, 4, 34, 30

; 	imprimir_texto_mp msg_debug_gs, 2, 0x70, 36, 27
; 	imprimir_hexa gs, 4, 36, 30

; 	imprimir_texto_mp msg_debug_ss, 2, 0x70, 38, 27
; 	imprimir_hexa ss, 4, 38, 30

; 	pushfd
; 	pop eax
; 	imprimir_texto_mp msg_debug_eflags, 6, 0x70, 40, 27
; 	imprimir_hexa eax, 8, 40, 30

; 	mov eax, cr0
; 	imprimir_texto_mp msg_debug_cr0, 3, 0x70, 10, 40
; 	imprimir_hexa cr0, 4, 10, 44

; 	mov eax, cr2
; 	imprimir_texto_mp msg_debug_cr2, 3, 0x70, 12, 40
; 	imprimir_hexa cr2, 4, 12, 44

; 	mov eax, cr3
; 	imprimir_texto_mp msg_debug_cr3, 3, 0x70, 14, 40
; 	imprimir_hexa cr3, 4, 14, 44

; 	mov eax, cr4
; 	imprimir_texto_mp msg_debug_cr4, 3, 0x70, 16, 40
; 	imprimir_hexa cr4, 4, 16, 44

; 	imprimir_texto_mp msg_debug_stack, 5, 0x70, 27, 40

; 	;pushad
; 	;pushfd
; 	;; --- TERMINE DE IMPRIMIR LOS REGISTROS ---

; 	ret