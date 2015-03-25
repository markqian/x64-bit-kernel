[bits 32]
    
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
 
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
 
ALIGN 4
IDT:
    .Length       dw 0
    .Base         dd 0
 
; Function to switch directly to long mode from real mode.
; Identity maps the first 2MiB.
; Uses Intel syntax.
 
; es:edi    Should point to a valid page-aligned 16KiB buffer, for the PML4, PDPT, PD and a PT.
; ss:esp    Should point to memory that can be used as a small (1 uint32_t) stack
 
SwitchToLongMode:
    ; Zero out the 16KiB buffer.
    ; Since we are doing a rep stosd, count should be bytes/4.   
    push edi                           ; REP STOSD alters DI.
    mov ecx, 0x1000
    xor eax, eax
    cld
    rep stosd
    pop edi                            ; Get DI back.
    
; zero out tables
    push edi
    push edx

    lea edx, [edi + 0x9000]
    mov eax, 0x0
.ZeroOutAllTable:
    mov [edi], eax	;zero out
    add edi, 4			
    cmp edi, edx	
    jb .ZeroOutAllTable

    pop edx
    pop edi

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Need to load each page table address into page directory.                                                            
;; Each page table is offset by 4kb and points to 2 mb of physical memory.                
;; each directory entry is 8 bytes with 512 entries. So in total 512 * 8 = 4096 bytes 
;; level 4 page map is located at edi + 0x0000                                                                                     
;; page directory pointer is located at edi + 0x1000                                                                              
;; first page directory is located at edi + 0x2000                                                                                   
;; identity map page table is located at edi + 0x3000                                                                                                 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; Build the Page Map Level 4.
    ; es:di points to the Page Map Level 4 table.    
    lea eax, [edi + 0x1000]         ; Put the address of the Page Directory Pointer Table in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writable flag.
    mov [edi], eax                  ; Store the value of EAX as the first PML4E.

    ; Build the Page Directory Pointer Table.
    lea eax, [edi + 0x2000]         ; Put the address of the Page Directory in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writable flag.
    mov [edi + 0x1000], eax         ; Store the value of EAX as the first PDPTE.

    ; Build the Page Directory.
    lea eax, [edi + 0x3000]         ; Put the address of the Page Table in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writeable flag.
    mov [edi + 0x2000], eax         ; Store to value of EAX as the first PDE.
   
 ; Build the Identity map.
    push edi                           ; Save DI for the time being.
    lea edi, [edi + 0x3000]             ; Point DI to the page table.
    mov eax, PAGE_PRESENT | PAGE_WRITE    ; Move the flags into EAX - and point it to 0x0000.	
 
.LoopIdentityPageTable:
    mov [edi], eax
    add eax, 0x1000
    add edi, 8
    cmp eax, 0x200000                 ; If we did all 2MiB, end.
    jb .LoopIdentityPageTable
 
    pop edi                            ; Restore DI.

    ;; build kernel map
    lea eax, [edi]        
    or eax, PAGE_PRESENT | PAGE_WRITE
    mov [edi+0xff0], eax 	;move the first entry of PML4 into its second last entry for recursive page mapping. 

    ;; fill last entry of  PML4 with address edi + 0x4000
    lea eax, [edi + 0x4000]         ; Put the address of the Page Directory Pointer Table in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writable flag.
    mov [edi+0xff8], eax                  ; Store the value of EAX as the last PML4E.

    ;; fill first entry of PDPT at edi + 0x4000
    lea eax, [edi + 0x5000]	
    or eax, PAGE_PRESENT | PAGE_WRITE ; need to change this to point to kernel memory address
    mov [edi + 0x4ff0], eax

    ;; fill first entry of PD
    lea eax, [edi + 0x6000]
    or eax, PAGE_PRESENT | PAGE_WRITE
    mov [edi + 0x5000], eax	

    ;; fill 2m of memory in PT
    push edi                           ; Save DI for the time being.
    lea edi, [edi + 0x6000]             ; Point DI to the page table.
    lea eax, [0x100000]
    or eax, PAGE_PRESENT | PAGE_WRITE    ; Move the flags into EAX - and point it to 0x100000.
        
.LoopKernelPageTable:
    mov [edi], eax
    add eax, 0x1000
    add edi, 8
    cmp eax, 0x300000
    jb .LoopKernelPageTable
    
    pop edi                            ; Restore DI.
    
    ; Disable IRQs
    mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out 0xA1, al
    out 0x21, al
 
    nop
    nop
 
    lidt [IDT]                        ; Load a zero length IDT so that any NMI causes a triple fault.
 
    ; Enter long mode.
    mov eax, 10100000b                ; Set the PAE and PGE bit.
    mov cr4, eax
 
    mov edx, edi                      ; Point CR3 at the PML4.
    mov cr3, edx
 
    mov ecx, 0xC0000080               ; Read from the EFER MSR. 
    rdmsr    
 
    or eax, 0x00000100                ; Set the LME bit.
    wrmsr
 
    mov ebx, cr0                      ; Activate long mode -
    or ebx,0x80000001                 ; - by enabling paging and protection simultaneously.
    mov cr0, ebx                    
 
    lgdt [GDT.Pointer]                ; Load GDT.Pointer defined below.
 
    jmp CODE_SEG:LongMode             ; Load CS with 64 bit segment and flush the instruction cache
 
; Global Descriptor Table
GDT:
.Null:
    dq 0x0000000000000000             ; Null Descriptor - should be present.
 
.Code:
    dw 0x0
    dw 0x0
    db 00000000b
    db 10011000b
    db 00100000b
    db 00000000b
    
.Data:    
    dw 0x0
    dw 0x0
    db 00000000b
    db 10010000b
    db 00000000b
    db 00000000b
    
ALIGN 4
    dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary
 
.Pointer:
    dw $ - GDT - 1                    ; 16-bit Size (Limit) of GDT.
    dd GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)
 
 
[BITS 64]      
LongMode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0x90000
    jmp BEGIN_LM                     ; You should replace this jump to wherever you want to jump to.
