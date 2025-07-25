GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_get_regs
GLOBAL sys_get_time
GLOBAL sys_delete_char
GLOBAL sys_get_key_state
GLOBAL sys_zoom_in
GLOBAL sys_zoom_out
GLOBAL sys_sleep
GLOBAL sys_start_beep
GLOBAL sys_stop_beep
GLOBAL sys_beep
GLOBAL sys_clear_screen
GLOBAL sys_draw_circle
GLOBAL sys_draw_rec
GLOBAL sys_fill_screen
GLOBAL sys_draw_pixel
GLOBAL sys_chars_width
GLOBAL sys_chars_height
GLOBAL sys_get_screen_width
GLOBAL sys_get_screen_height
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

sys_get_regs:
    sysCallMaster 2

sys_get_time:
    sysCallMaster 3

sys_delete_char: 
    sysCallMaster 4

sys_get_key_state:
    sysCallMaster 5

sys_zoom_in:
    sysCallMaster 6

sys_zoom_out:
    sysCallMaster 7

sys_sleep:
    sysCallMaster 23

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

sys_draw_rec:
    sysCallMaster 32

sys_fill_screen:
    sysCallMaster 33

sys_draw_pixel:
    sysCallMaster 34

sys_chars_width:
    sysCallMaster 35

sys_chars_height:
    sysCallMaster 36

sys_get_screen_width:
    sysCallMaster 37

sys_get_screen_height:
    sysCallMaster 38
    