[org 0x7c00]
    %define KERNEL_OFFSET 0x1000
    %define FREE_SPACE 0x9000

Main:
    jmp 0x0000:.FlushCS               ; Some BIOS' may load us at 0x0000:0x7C00 while other may load us at 0x07C0:0x0000.

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

    mov [BOOT_DRIVE], dl	
    call print_hex
     	    
    mov sp, 0x8000

    call load_kernel
    
    jmp switch_to_pm
    
    jmp $

%include "print_string.asm"
%include "print_hex.asm"
%include "mydisk_load.asm"
%include "gdt.asm"
%include "print_string_long.asm"
%include "switch_to_pm.asm"
%include "switch_to_long_mode.asm"

[bits 32]
 
NoLongMode db "ERROR: CPU does not support long mode.", 0x0A, 0x0D, 0
  
; Checks whether CPU supports long mode or not.
; Returns with carry set if CPU doesn't support long mode.
 
CheckCPU:
    ; Check whether CPUID is supported or not.
    pushfd                            ; Get flags in EAX register.
 
    pop eax
    mov ecx, eax  
    xor eax, 0x200000 
    push eax 
    popfd
 
    pushfd 
    pop eax
    xor eax, ecx
    shr eax, 21 
    and eax, 1                        ; Check whether bit 21 is set or not. If EAX now contains 0, CPUID isn't supported.
    push ecx
    popfd 
 
    test eax, eax
    jz .NoLongMode
 
    mov eax, 0x80000000   
    cpuid                 
 
    cmp eax, 0x80000001               ; Check whether extended function 0x80000001 is available are not.
    jb .NoLongMode                    ; If not, long mode not supported.
 
    mov eax, 0x80000001  
    cpuid                 
    test edx, 1 << 29                 ; Test if the LM-bit, is set or not.
    jz .NoLongMode                    ; If not Long mode not supported.
 
    ret
 
.NoLongMode:
    stc
    ret
 
 
; Prints out a message using the BIOS.
 ; es:si    Address of ASCIIZ string to print.
 
Print:
    pushad
.PrintLoop:
    lodsb                             ; Load the value at [@es:@si] in @al.
    test al, al                       ; If AL is the terminator character, stop printing.
    je .PrintDone                  	
    mov ah, 0x0E	
    int 0x10
    jmp .PrintLoop                    ; Loop till the null character not found.
 
.PrintDone:
    popad                             ; Pop all general purpose registers to save them.
    ret

[bits 16]    
load_kernel:
    pusha
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov dl, [BOOT_DRIVE]
    call mydisk_load

    mov dx, [KERNEL_OFFSET]
    call print_hex

    popa
    ret
    
[bits 64]
    
BEGIN_LM:
    mov ebx, MSG_LONG_MODE
    call print_string_long
    
    call KERNEL_OFFSET
    jmp $

    
BOOT_DRIVE:  db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0
MSG_LONG_MODE db "Successfully landed in Long Mode", 0

MSG_LOAD_KERNEL db "Loading kernel into memory", 0

    times 2048-($-$$) db 0xff	

