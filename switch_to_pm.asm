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


    ;; load kernel elf header
    ;; load memory based on program header.    
    ;; need program offset
    ;; need program segment size
    mov bx, [elf_offset+e_phentsize_offset]
    mov cx, [elf_offset+e_phnum_offset]

    mov eax, program_offset

.LoadKernelSegments:
    ;; eax contains the address of the wanted program header
    call load_kernel_segment	
    add eax, ebx
    sub cx, 1
    cmp cx, 0
    ja .LoadKernelSegments
  
    ;; copy kernel to higher address
    ;; go back to real mode if there are still sectors left to be loaded
    
    mov edi, FREE_SPACE
    
    call SwitchToLongMode

    ;; assumes that eax contains address of the program header
load_kernel_segment:
    push eax
    push ebx
    push ecx
    push edx

    mov ebx, 0x10000

    ;;  this assumes that each page is 4k
    lea edx, [0x100000 - 0x1000]

    add ebx, [eax + p_offset_offset]
    add edx, [eax + p_offset_offset]
    mov ecx, 0

    cmp ecx, [eax + p_memsz_offset]
    je .load_kernel_segment_done

    ;; if p_filesz < memsz zero out the rest of the memory

    push ebx
    push ecx
    push edx

    mov ebx, 0
    
.ZeroOutRegion:
    mov [edx], ebx
    add ecx, 4
    add edx, 4
    cmp ecx, [eax + p_memsz_offset]
    jb .ZeroOutRegion

    pop edx
    pop ecx
    pop ebx
    
.MoveKernelMemory:
    push eax

    mov eax, [ebx]
    mov [edx], eax
    add ebx, 4
    add edx, 4
    add ecx, 4

    pop eax
    
    cmp ecx, [eax + p_filesz_offset]	
    jb .MoveKernelMemory


.load_kernel_segment_done:    
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    ret
        
elf_offset equ 0x10000
program_offset equ 0x10040
    ;; elf header offsets
e_indent_offset equ 0
e_type_offset equ 16
e_machine_offset equ 18
e_version_offset equ 20
e_entry_offset equ 24
e_phoff_offset equ 32
e_shoff_offset equ 40
e_flags_offset equ 48
e_ehsize_offset equ 52
e_phentsize_offset equ 54
e_phnum_offset equ 56
e_shentsize_offset equ 58
e_shnum_offset equ 60
e_shstrndx_offset equ 62

    ;; program header offsets
p_type_offset equ 0	
p_flags_offset equ 4	
p_offset_offset equ 8	
p_vaddr_offset equ 16	
p_paddr_offset equ 24	
p_filesz_offset equ 32	
p_memsz_offset equ 40
p_align_offset equ 48
