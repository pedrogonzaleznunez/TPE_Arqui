
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

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN save_registers_flag
EXTERN saved_registers
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
	popState ; restauro
	mov [saved_registers.rax], rax

	mov rax, [rsp+8*2] ; rflags
	mov [saved_registers.rflags], rax 

	mov rax, [rsp+8*3] ; rsp de donde vengo
	mov [saved_registers.rsp], rax 

	mov rax, [rsp] ; eip de donde vengo
	mov [saved_registers.rip], rax ; guardo la direccion de retorno

    mov [saved_registers.rbx], rbx
    mov [saved_registers.rcx], rcx
    mov [saved_registers.rdx], rdx
    mov [saved_registers.rsi], rsi
    mov [saved_registers.rdi], rdi
    mov [saved_registers.rbp], rbp
    mov [saved_registers.r8], r8
    mov [saved_registers.r9], r9
    mov [saved_registers.r10], r10
    mov [saved_registers.r11], r11
    mov [saved_registers.r12], r12
    mov [saved_registers.r13], r13
    mov [saved_registers.r14], r14    
	mov [saved_registers.r15], r15
	mov rax, [saved_registers.rax]

.skips_save:
	pushState
	mov byte [save_registers_flag], 0

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


; Exceptions handler
%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
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
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1