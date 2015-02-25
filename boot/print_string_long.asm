[bits 64]

print_string_long:
    push rdx
    push rax
    mov rdx, 0xb8000    
print_string_long_loop:
    mov al, [rbx]
    mov ah, 0x0f

    cmp al, 0
    je print_string_long_done

    mov [rdx], ax

    add rbx, 1    
    add rdx, 2

    jmp print_string_long_loop

print_string_long_done:
    pop rax
    pop rdx

    ret
