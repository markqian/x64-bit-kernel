[bits 64]
    
[GLOBAL gdt_load]

    %define KCODE_SEG 0x0008
    %define KDATA_SEG 0x0010

gdt_load:

    lgdt [rdi]
    
    mov rax, KDATA_SEG
    mov ds, ax
    mov es, ax
    mov fs,  ax
    mov gs, ax
    mov ss, ax
    
    mov rsi, rsp
    mov rcx, .flush
    
    push rax				;set DATA_SEG for iretq
    push rsi				;set RSP for iretq
    pushfq					;set EFLAG for iretq
    push QWORD KCODE_SEG	;set CODE_SEG for iretq
    push rcx				;far jump address for iretq
    iretq

.flush:
    ret

[GLOBAL idt_load]

idt_load:
    lidt [rdi]
    ret
    

