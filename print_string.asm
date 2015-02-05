[bits 16]
print_string:
     pusha
     mov ah, 0x0e
print_string_loop:
     mov dl, [bx]
     cmp dl, 0
     je print_string_end			
     mov al, [bx]    
     int 0x10
     add bx, 1
     jmp print_string_loop
print_string_end:	
     popa
     ret
    
