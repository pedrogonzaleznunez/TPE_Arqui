GLOBAL throw_zero_division
GLOBAL throw_invalid_opcode

section .text

throw_zero_division:
    mov rax, 0
    div rax
    ret

throw_invalid_opcode:
    ud2 ; instrucción que se usa para generar la excepción
    ret
