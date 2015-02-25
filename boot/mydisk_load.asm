BootCatalogSectorOffset equ 0x47
BootLoaderOffset equ 0x28    

mydisk_load:
    pusha

    ; Load CD descriptor at sector 17
    mov si, DescriptorAddressPacket
    mov ah,0x42			
    int 0x13
    jc disk_error

    ; Get boot catalog sector offset
    push es
    mov bx, 0xf00
    mov es, bx
    mov bx, [es:0x1047]
    pop es

    
    ; Load boot catalog sector
    mov [BootCatalogSector], bx
    mov si, BootCatalogAddressPacket
    mov ah,0x42			
    int 0x13
    jc disk_error

    
    ; Read disk sector offset
    push es
    mov bx, 0xf00
    mov es, bx
    mov bx, [es:0x1028]
    pop es

    push dx
    mov dx, bx
    call print_hex
    pop dx


    ;; Get sector below, which is where the kernel is located
    add bx, 1
    
    ; Load disk sector 
    mov [DiskSector], bx
    mov si, DiskAddressPacket
    mov ah,0x42			
    int 0x13
    jc disk_error

    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

success:
    pusha
    mov bx, DISK_SUCCESS_MSG
    call print_string
    popa
    ret
    
DiskAddressPacket:        
DiskSize:   	db 0x10				;size of packet
DiskZero:   	db 0					;always 0
DiskTransfers:	dw 155				;number of sectors to read
DiskOffset: 	dw KERNEL_OFFSET		;offset
DiskSegment:	dw 0xf00				;segment
DiskSector: 	dd 0					;starting LBA #
DiskUpper:  	dd 0   				;used for upper part of 48 bit LBAs

DescriptorAddressPacket:        
DescriptorSize:		db 0x10				;size of packet
DescriptorZero:	db 0					;always 0
DescriptorTransfers:	dw 1				;number of sectors to read
DescriptorOffset:   	dw KERNEL_OFFSET		;offset
DescriptorSegment:  dw 0xf00				;segment
DescriptorSector:   	dd 17				;starting LBA #
DescriptorUpper:    	dd 0   				;used for upper part of 48 bit LBAs

BootCatalogAddressPacket:        
BootCatalogPacketSize:	db 0x10	 			;size of packet
BootCatalogZero:    		db 0		 			;always 0
BootCatalogTransfers:	dw 1	 			;number of sectors to read
BootCatalogOffset: 	     	dw KERNEL_OFFSET 	;offset
BootCatalogSegment:    	dw 0xf00	 			;segment
BootCatalogSector:	    	dd 0          			;starting LBA #
BootCatalogUpper:   	dd 0   	 			;used for upper part of 48 bit LBAs
    
    
DISK_ERROR_MSG:	db "Disk read error!",0
DISK_SUCCESS_MSG:	db "Disk read success!",0
