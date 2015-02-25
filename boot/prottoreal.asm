[org 0x7c00]
    
[bits 16]
Main:
    jmp 0x0000:.FlushCS               

.FlushCS:
    xor ax, ax
    ; Set up segment registers.
    mov ss, ax
    ; Set up stack so that it starts below Main.
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    cld

    mov sp, Main

    mov bx, MSG_REAL_MODE
    call print_string

switch_to_pm:
    cli	
    lgdt [gdt_descriptor]
    
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    jmp CODE_SEG:init_pm
    
[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call 0x18:move_to_real
    jmp $

move_to_real:
   mov eax, 0x20
   mov ds, eax
   mov es, eax
   mov fs, eax
   mov gs, eax
   mov ss, eax
   
   mov eax, cr0
   and al, 0
   mov cr0, eax

[bits 16] 
   jmp 0:real_mode
    
[bits 16]
real_mode:
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov bx, MSG_REAL_MODE
    call print_string

    jmp $

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

    
%include "print_string.asm"
%include "print_string_pm.asm"

MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0
    
times 510-($-$$) db 0
dw 0xaa55
