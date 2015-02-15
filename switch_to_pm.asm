[bits 16]
; Switch to protected mode
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


    ;; parse kernel elf header
    ;; parse kernel program header
    ;; load memory based on program header.
    call parse_kernel_elf
    
    ;; need program offset
    ;; need program segment size
    mov bx, [kernel_elf.e_phentsize]
    mov ax, [kernel_elf.e_phnum]

    ;; step 1 parse program header
    ;; step 2 move segment to memory
    ;; go back to step 1
    mov ecx, 1 			;set up counter for parsing program headers
.LoadKernelSegments:
    call parse_program_header
    call load_kernel_segment	
    add ecx, 1
    sub ax, 1
    cmp ax, 0
    ja .LoadKernelSegments
  
    ;; copy kernel to higher address
    ;; go back to real mode if there are still sectors left to be loaded
    
    mov edi, FREE_SPACE
    
    call SwitchToLongMode

load_kernel_segment:
    push eax
    push ebx
    push ecx
    push edx
    
    mov eax, 0x10000
    ;;  this assumes that each page is 4k
    lea edx, [0x100000 - 0x1000]

    add eax, [program_header.p_offset]
    add edx, [program_header.p_offset]
    mov ecx, 0

    cmp ecx, [program_header.p_memsz]
    je .load_kernel_segment_done
        
.MoveKernelMemory:
    mov ebx, [eax]
    mov [edx], ebx
    add eax, 4
    add edx, 4
    add ecx, 4
    cmp ecx, [program_header.p_memsz]	
    jb .MoveKernelMemory

    ;; if p_filesz < memsz zero out the rest of the memory

.load_kernel_segment_done:    
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    ret
    
parse_kernel_elf:
    push eax
    push ebx
    push ecx
    push edx
    
    mov ecx, 0
    mov eax, 0x10000

fill_elf:
    mov edx, [eax]
    mov [kernel_elf + ecx*4], edx
    add eax, 4
    add ecx, 1
    cmp ecx, 16
    jb fill_elf

    pop edx
    pop ecx
    pop ebx
    pop eax
    
    ret

    ;; ebx will hold value for program header length in bytes
    ;; ecx will hold value for program header number
parse_program_header:
    push eax
    push ebx
    push ecx
    push edx
    
    mov edx, 0
    mov eax, 0
get_header_offset:	
    add eax, edx
    add edx, ebx
    sub ecx, 1
    cmp ecx, 0
    ja get_header_offset
    
    add eax, 0x40

    push ebx
    mov ebx, eax	
    ;; ebx now contains offset the beginning of the wanted program header
    mov eax, 0x10000
    add eax, ebx
    ;; ebx now contains length of program header in bytes
    pop ebx

    ;; eax contains the address of the wanted program header
    mov edx, 0
fill_header:
    mov cx, [eax]
    mov [program_header+edx], cx
    add eax, 1
    add edx, 1
    cmp edx, ebx
    jne fill_header

    pop edx
    pop ecx
    pop ebx
    pop eax

    ret
    
program_header:	
    .p_type:	dd 0
    .p_flags:	dd 0
    .p_offset:	db 0,0,0,0,0,0,0,0
    .p_vaddr:	db 0,0,0,0,0,0,0,0
    .p_paddr:	db 0,0,0,0,0,0,0,0
    .p_filesz:	db 0,0,0,0,0,0,0,0
    .p_memsz:	db 0,0,0,0,0,0,0,0
    .p_align:	db 0,0,0,0,0,0,0,0

    
kernel_elf:
    .e_indent: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  
    .e_type: dw 0
    .e_machine: dw 0
    .e_version: dd 0
    .e_entry: db 0,0,0,0,0,0,0,0
    .e_phoff: db 0,0,0,0,0,0,0,0
    .e_shoff: db 0,0,0,0,0,0,0,0
    .e_flags: dd 0
    .e_ehsize: dw 0
    .e_phentsize: dw 0
    .e_phnum: dw 0
    .e_shentsize: dw 0
    .e_shnum: dw 0
    .e_shstrndx: dw 0

