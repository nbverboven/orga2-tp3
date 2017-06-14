; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

extern GDT_DESC
extern IDT_DESC

extern idt_inicializar
extern pintar_pantalla
extern mmu_inicializar
extern mmu_inicializar_dir_kernel

%macro pintar_pantalla 0
    imprimir_texto_mp pintar_pantalla_msg, 4000, 0x22, 0, 0

    mov ecx, 44
    mov ecx, ecx

    pintarBarras:
        imprimir_texto_mp pintar_pantalla_msg, 1, 11001100b, ecx, 0
        imprimir_texto_mp pintar_pantalla_msg, 1, 10011001b, ecx, 79 
    loop pintarBarras

    imprimir_texto_mp pintar_pantalla_msg, 80, 0x00, 0, 0
    imprimir_texto_mp pintar_pantalla_msg, 400, 0x00, 45, 0

    imprimir_texto_mp pintar_pantalla_nros, pintar_pantalla_nros_len, 0x0F, 46, 5
    imprimir_texto_mp pintar_pantalla_nros, pintar_pantalla_nros_len, 0x0F, 46, 60


    mov ecx, 5
    mov ecx, ecx

    pintarCuadrados:
        mov eax, ecx
        add eax, 44
        imprimir_texto_mp pintar_pantalla_msg, 5, 01000100b, eax, 34
        imprimir_texto_mp pintar_pantalla_msg, 5, 00010001b, eax, 39
    loop pintarCuadrados

    imprimir_texto_mp pintar_pantalla_group_name, pintar_pantalla_group_name_len, 0xBF, 0, 65 
    mov eax, 9733
    push eax
    imprimir_texto_mp esp, 1, 0xBF, 0, 76 
    pop eax

%endmacro


;; Saltear seccion de datos
jmp start

%define PILA_KERNEL         0x27000
%define BASE_PAGE_DIRECTORY 0x27000

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_empty_msg db     ' '
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

pintar_pantalla_msg         db ''
pintar_pantalla_nros        db '1 2 3 4 5 6 7 8'
pintar_pantalla_nros_len equ $ - pintar_pantalla_nros

pintar_pantalla_group_name  db 'Estrellitas'
pintar_pantalla_group_name_len equ $ - pintar_pantalla_group_name

barra_len equ $ - 1

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1 
    mov cr0,eax

    ; Saltar a modo protegido
    jmp 0x0040:modoprotegido

BITS 32
modoprotegido:

;xchg bx,bx ;breakpoint

    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, 0x0050 ; 10<<3 1010000b 0x50
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov ax, 0x0060 ; 1100000b
    mov fs, ax

    ; Establecer la base de la pila
    mov eax, PILA_KERNEL
    mov ebp, eax
    mov esp, eax

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    pintar_pantalla

    ; Inicializar el manejador de memoria
    call mmu_inicializar
    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, BASE_PAGE_DIRECTORY
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt[IDT_DESC]
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
