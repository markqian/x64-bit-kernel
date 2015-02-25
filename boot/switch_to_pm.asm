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
    
    ;; need program header size
    mov bx, [elf_offset+e_phentsize_offset]
    ;; need number of program headers 
    mov cx, [elf_offset+e_phnum_offset]

    mov eax, program_offset
    mov edx, 0x0
        
.GetKernelMemSize:
    ;; calculate total memsz; copy memory to 0x100000
    ;; eax contains the address of the wanted program header
    
    add edx, [eax + p_memsz_offset]
    add eax, ebx
    sub cx, 1
    cmp cx, 0
    ja .GetKernelMemSize

    mov ebx, 0x11000
    mov eax, 0x100000
    mov ecx, 0

    ;; edx contain the total amount of memory that needs to be loaded
.LoadKernel:
    push edx
    mov edx, [ebx]
    mov [eax], edx
    pop edx
    
    add ebx, 4 			;if ebx == 0x9B000, load more memory starting at 0x11000
    add eax, 4			
    add ecx, 4			;ecx contains memory that has been loaded
    cmp ecx, edx
    jb .LoadKernel
       
    mov edi, FREE_SPACE
    
    call SwitchToLongMode

    
elf_offset 			equ 0x10000
program_offset 	equ 0x10040
    ;; elf header offsets
e_indent_offset 	equ 0
e_type_offset 		equ 16
e_machine_offset 	equ 18
e_version_offset 	equ 20
e_entry_offset 		equ 24
e_phoff_offset 		equ 32
e_shoff_offset 		equ 40
e_flags_offset 		equ 48
e_ehsize_offset 	equ 52
e_phentsize_offset 	equ 54
e_phnum_offset 	equ 56
e_shentsize_offset 	equ 58
e_shnum_offset 	equ 60
e_shstrndx_offset 	equ 62

    ;; program header offsets
p_type_offset 		equ 0	
p_flags_offset 		equ 4	
p_offset_offset 		equ 8	
p_vaddr_offset 		equ 16	
p_paddr_offset 	equ 24	
p_filesz_offset 		equ 32	
p_memsz_offset 	equ 40
p_align_offset 		equ 48
