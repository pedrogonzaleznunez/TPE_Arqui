GLOBAL _syscallHandler

EXTERN syscallDispatcher

SECTION .text

%macro pushState 0
	push rbp
	push rbx
	push r12
	push r13
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r13
	pop r12
	pop rbx
	pop rbp
%endmacro

_syscallHandler:
	push rbp
	mov rbp, rsp
	pushState
    
	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax

	call syscallDispatcher
	
	popState
	mov rsp, rbp
	pop rbp

	iretq
