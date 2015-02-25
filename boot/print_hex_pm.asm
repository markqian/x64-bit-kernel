[bits 32]
    
print_hex_pm:
    push edx
    push ebx
    push ecx
 
    mov ebx, 9
print_hex_pm_loop:
    cmp ebx, 1
    je print_hex_pm_end
    mov cl, dl
    and cl, 0xf
    shr edx, 4
print_hex_pm_is_number:	
    cmp cl, 0xa
    jge print_hex_pm_is_alpha
    add cl, '0'
    mov [HEX_OUT_PM + ebx], cl
    sub ebx, 1
    jmp print_hex_pm_loop
print_hex_pm_is_alpha:
    add cl, 'W'
    mov [HEX_OUT_PM + ebx], cl
    sub ebx, 1
    jmp print_hex_pm_loop        
print_hex_pm_end:	
    mov ebx, HEX_OUT_PM
    call print_string_pm

    pop ecx
    pop ebx
    pop edx
    ret

HEX_OUT_PM:     db '0x00000000',0

