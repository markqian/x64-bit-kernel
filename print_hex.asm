print_hex:
    pusha
    mov bx, 5
print_hex_loop:
    cmp bx, 1
    je print_hex_end
    mov cl, dl
    and cl, 0xf
    shr dx, 4
print_hex_is_number:	
    cmp cl, 0xa
    jge print_hex_is_alpha
    add cl, '0'
    mov [HEX_OUT + bx], cl
    sub bx, $1
    jmp print_hex_loop
print_hex_is_alpha:
    add cl, 'W'
    mov [HEX_OUT + bx], cl
    sub bx, $1
    jmp print_hex_loop        
print_hex_end:	
    mov bx, HEX_OUT
    call print_string
    popa
    ret

HEX_OUT:     db '0x0000',0
