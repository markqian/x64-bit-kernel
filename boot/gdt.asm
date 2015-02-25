[bits 16]
gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

; 16 bit code (0x18)
   dw 0xffff
   dw 0x0000
   db 0x00
   db 0x9A
   db 0xf
   db 0x00
; 16 bit data (0x20)
   dw 0xffff
   dw 0x0000
   db 0x00
   db 0x92
   db 0xf
   db 0x00    
    
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
    
