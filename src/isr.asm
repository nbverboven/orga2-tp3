; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

int_0_msg db 'Devide error wacho!'
int_0_len equ $ - int_0_msg

int_1_msg db 'RESERVED vieja!'
int_1_len equ $ -int_1_msg

int_2_msg db 'NMI interrupt!'
int_2_len equ $ -int_2_msg

int_3_msg db 'Breakpoint wachin!'
int_3_len equ $ -int_3_msg

int_4_msg db 'Overflow, rescatate!'
int_4_len equ $ -int_4_msg

int_5_msg db 'BOUND Range Exceeded, manejate!'
int_5_len equ $ -int_5_msg

int_6_msg db 'Invalid Opcode (Undefined Opcode), hiciste cualquiera!'
int_6_len equ $ -int_6_msg

int_7_msg db 'Device Not Available (No Math Coprocessor), tah re loko!'
int_7_len equ $ -int_7_msg

int_8_msg db 'Double Fault, jodete!'
int_8_len equ $ -int_8_msg

int_9_msg db 'Coprocessor Segment Overrun (reserved)...!'
int_9_len equ $ -int_9_msg

int_10_msg db 'Invalid TSS, vo fijateh!'
int_10_len equ $ -int_10_msg

int_11_msg db 'Segment Not Present, anda a buscarlo!'
int_11_len equ $ -int_11_msg

int_12_msg db 'Stack-Segment Fault, cosas que pasan!'
int_12_len equ $ -int_12_msg

int_13_msg db  'General protection fault!!!!!!, gil'
int_13_len equ $ - int_13_msg

int_14_msg db 'Page Fault, arreglalo!'
int_14_len equ $ -int_14_msg

int_16_msg db 'x87 FPU Floating-Point Error (Math Fault), mira bien lo que haces!'
int_16_len equ $ -int_16_msg

int_17_msg db 'Alignment Check! :-)'
int_17_len equ $ -int_17_msg

int_18_msg db 'Machine Check! ;)'
int_18_len equ $ -int_18_msg

int_19_msg db 'SIMD Floating-Point Exception! :('
int_19_len equ $ -int_19_msg


;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 0
global _isr%0

_isr%0:
    imprimir_texto_mp  int_0_msg, int_0_len, 0x07, 0, 0
    jmp $

%endmacro

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    jmp $

%endmacro

%macro ISR 13
global _isr%13

_isr%13:
    imprimir_texto_mp  int_13_msg, int_13_len, 0x07, 0, 0
    jmp $

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

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

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


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
        
        
