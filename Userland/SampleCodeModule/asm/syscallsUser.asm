GLOBAL sys_call

SECTION .text

; ver si conviene poner en la pos 80 una sys call que recibe un numero y llama a l

; recibe los parametros por registros (arqui 64 bits)
sys_call:
    
    mov rax, rdi    ; syscall ID
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    mov r8, r9
    
    int 80h ; es una instruccion de asm que va a posicion 80 de la tabla idt
    
    ret
