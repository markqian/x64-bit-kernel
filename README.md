# x64-bit-kernel

stuff that I need to do:

-set base address of kernel to be 0xffffffff80000000			    #done

-change kernel physical address to start at 0x100000	            #done

-figure out how to switch from protected mode to real mode         #done

-store paging stuff starting at address 0x9000			            #done

-enable A20 mode                                                                         #done


-need to load second stage bootloader

-need to setup recursive paging						    #done

-need to figure out how to implement keyboard driver, screen driver and file systems.  

-read up on linker scripts to understand how each segment is being mapped.  
