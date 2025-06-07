GLOBAL throwZeroDivisionException
GLOBAL throwInvalidOpcodeException

section .text

throwZeroDivisionException:
    mov rax, 0
    div rax
    ret

throwInvalidOpcodeException:
    ud2 ; instrucción que se usa para generar la excepción
    ret
