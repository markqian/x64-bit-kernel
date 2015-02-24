[bits 64]
[GLOBAL kprint_hex]
    
kprint_hex:
    push rdx
    push rbx
    push rcx
    
    mov cl, 'x'
    mov ch, 0x0f
    mov [rdi + 2], cx

    mov cl, '0'
    mov ch, 0x0f
    mov [rdi + 0], cx

    mov rbx, 34
kprint_hex_loop:
    cmp rbx, 2
    je kprint_hex_end
    mov cl, sil
    and cl, 0xf
    shr rsi, 4
    
kprint_hex_is_number:	
    cmp cl, 0xa
    jge kprint_hex_is_alpha
    add cl, '0'

    mov ch, 0x0f
    mov [rdi + rbx], cx
    sub rbx, 2

    jmp kprint_hex_loop
    
kprint_hex_is_alpha:
    add cl, 'W'
    mov ch, 0x0f
    
    mov [rdi + rbx], cx
    sub rbx, 2

    jmp kprint_hex_loop        
kprint_hex_end:

    pop rcx
    pop rbx
    pop rdx
    ret

