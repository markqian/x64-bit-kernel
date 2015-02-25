[bits 64]
    
print_hex_long:
    push rdx
    push rbx
    push rcx
 
    mov rbx, 17
print_hex_long_loop:
    cmp rbx, 1
    je print_hex_long_end
    mov cl, dl
    and cl, 0xf
    shr rdx, 4
print_hex_long_is_number:	
    cmp cl, 0xa
    jge print_hex_long_is_alpha
    add cl, '0'
    mov [HEX_OUT_LONG + rbx], cl
    sub rbx, 1
    jmp print_hex_long_loop
print_hex_long_is_alpha:
    add cl, 'W'
    mov [HEX_OUT_LONG + rbx], cl
    sub rbx, 1
    jmp print_hex_long_loop        
print_hex_long_end:	
    mov rbx, HEX_OUT_LONG
    call print_string_long

    pop rcx
    pop rbx
    pop rdx
    ret

HEX_OUT_LONG:     db '0x0000000000000000',0


    
