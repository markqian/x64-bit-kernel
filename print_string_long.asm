[bits 64]
    
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_long:
    push rdx
    push rax
    mov rdx, VIDEO_MEMORY

print_string_long_loop:
    mov al, [rbx]
    mov ah, WHITE_ON_BLACK

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
