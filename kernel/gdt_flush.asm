[bits 64]
[GLOBAL gdt_flush]
    %define CODE_SEG     0x0008
    %define DATA_SEG     0x0010

gdt_flush:
    mov rax, [rsp + 4]
    lgdt [rax]

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
