GLOBAL cpuVendor
GLOBAL getTimeFragment
GLOBAL getKey

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; En rdi recibe el tipo de dato que se necesita:
; 0 son los segundos, 2 los minutos, 4 las horas,
; 6 el día de la semana, 7 el día del mes, 8 el mes y 9 el año
; https://stanislavs.org/helppc/cmos_ram.html
getTimeFragment:
	push rbp
	mov rbp, rsp

	xor rax, rax

	cmp rdi, 0
	jl .end_1
	cmp rdi, 9
	jg .end_1

	mov al, dil
    out 0x70, al        ; selecciona registro
    in al, 0x71         ; lee contenido
    movzx rax, al       ; extiende valor a 64 bits

.end_1:
	mov rsp, rbp
	pop rbp
	ret


getKey:
   push rbp
   mov rbp, rsp

   	in al, 0x60
	movzx rax, al
    
	mov rsp, rbp
   pop rbp
   ret

