GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_start_beep
GLOBAL sys_beep
GLOBAL sys_stop_beep
GLOBAL sys_sleep
GLOBAL sys_clear_screen
GLOBAL sys_draw_circle

SECTION .text

%macro sysCallMaster 1
    
    mov rax, %1
    int 80h
    ret

%endmacro

; ############### SYSTEM SYS_CALLS ###############
sys_write:
    sysCallMaster 0

sys_read:
    sysCallMaster 1

sys_get_regs
    sysCallMaster 2

; ################ SOUND SYS_CALLS ################
sys_start_beep:
    sysCallMaster 20

sys_stop_beep:
    sysCallMaster 21

sys_beep:
    sysCallMaster 22

; ################ VIDEO SYS_CALLS ################
sys_clear_screen:
    sysCallMaster 30

sys_draw_circle:
    sysCallMaster 31
