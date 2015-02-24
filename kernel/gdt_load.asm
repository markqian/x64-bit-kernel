[bits 64]
[GLOBAL gdt_load]

    %define CODE_SEG 0x0008
    %define DATA_SEG 0x0010

gdt_load:

    lgdt [rdi]
    
    mov rax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs,  ax
    mov gs, ax
    mov ss, ax
    
    mov rsi, rsp
    mov rcx, .flush
    
    push rax				;set DATA_SEG
    push rsi				;set RSP
    pushfq					;set EFLAG
    push QWORD CODE_SEG	;set CODE_SEG
    push rcx				;far jump address
    iretq

.flush:
    ret
