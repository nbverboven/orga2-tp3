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

%macro pintar_pantalla 0
    ;mov eax, 50          ; eax = fila
    ;mov ebx, 80
    ;imul ebx            ; eax = fila * 80
    ;mov ebx, 0          ; ebx = columna
    ;add eax, 80        ; eax = fila * 80 + columna

    ; Establecer el puntero al buffer de pantalla en (fila, columna)
    ;mov ebx, 0xb8000    ; ebx = puntero a la base del buffer de video
    ;shl eax, 1          ; Cada posicion de la memoria de video ocupa un word
    ;add ebx, eax        ; ebx = puntero a la posicion (fila, columna)

    ; Acomodar color en la parte alta de ax
    ;mov eax,            ; eax = color
    ;shl ax, 8           ; ah = color
    ;mov ax, 0

    ; Imprimir cadena
    ;%%ciclo_cadena:
    ;    mov     al, [edi]       ; al = caracter
    ;    mov     [ebx], ax       ; Escribir en pantalla
    ;    add     ebx, 2          ; Avanzar el puntero de la memoria de video
    ;    inc     edi             ; Avanzar el puntero de la cadena
    ;    loop    %%ciclo_cadena
    mov ecx, 49
fila:
    mov ebx, 79
columna:
    imprimir_texto_mr iniciando_empty_msg, 1, 0x07, ecx, ebx
    dec ebx
    loop columna
    dec ecx
    cmp ecx, 0
    jne fila

%endmacro

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_empty_msg db     ' '
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

%define pila_kernel 0x27000

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
    mov eax, pila_kernel
    mov ebp, eax
    mov esp, eax

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    pintar_pantalla

    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
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
