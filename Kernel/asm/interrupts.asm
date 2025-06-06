GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

; flags que indican:
; i) si se debe guardar el estado
; ii) si el estado se guardó
GLOBAL save_registers_flag
GLOBAL saved_registers_flag
; structs de los registros
EXTERN saved_registers
EXTERN saved_registers_on_exception

GLOBAL _exception00Handler
GLOBAL _exception06Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN getStackBase

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro saveRegisters 1
	mov [%1 + 8*0], rax
    mov [%1 + 8*1], rbx
    mov [%1 + 8*2], rcx
    mov [%1 + 8*3], rdx
    mov [%1 + 8*4], rsi
    mov [%1 + 8*5], rdi
    
	mov rax, [rsp+8*3] ; rsp de donde vengo
    mov [%1 + 8*6], rax 

    mov [%1 + 8*7], rbp
    mov [%1 + 8*8], r8
    mov [%1 + 8*9], r9
    mov [%1 + 8*10], r10
    mov [%1 + 8*11], r11
    mov [%1 + 8*12], r12
    mov [%1 + 8*13], r13
    mov [%1 + 8*14], r14    
    mov [%1 + 8*15], r15
    mov rax, [rsp+8*2] ; rflags
    mov [%1 + 8*16], rax 
    
    mov rax, [rsp] ; eip de donde vengo
    mov [%1 + 8*17], rax ; guardo la direccion de retorno

    mov rax, [%1 + 8*0]
%endmacro

; Interruption handler
%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

 		; check por si tengo que guardar los registros
    mov al, byte [save_registers_flag]
    cmp al, 0
   
    je .skip_save 
		; save:
    popState		; restauro
	saveRegisters saved_registers
	pushState
	
	mov byte[saved_registers_flag], 1		;indico que se guardaron

.skip_save:
    
    mov byte [save_registers_flag], 0

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


; Exceptions handler
%macro exceptionHandler 1
	saveRegisters saved_registers_on_exception

	; también se puede tener una única estructura
	; y que si hay una excepción, se pierden valores
	; guardados previos
	mov byte[saved_registers_flag], 0

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	; llamar a getStackBase y volver
	call getStackBase
	mov [rsp + 8*3], rax	; se guarda el rsp 
	mov rax, userland
	mov [rsp], rax			; se guarda el rip
	
	; rflags, cs y ss quedan como estaban
	
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception00Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception06Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1

SECTION .data
    save_registers_flag db 0
	saved_registers_flag db 0
	userland equ 0x400000