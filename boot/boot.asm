[bits 16]
[org 0x7c00]

mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

mov si, MSG_REAL_MODE
call print_string

call load_kernel

call switch_to_pm

print_string:
    mov ah, 0x0e
.loop:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

load_kernel:
    mov ah, 0x02    
    mov al, 15      
    mov ch, 0       
    mov cl, 2       
    mov dh, 0       
    mov bx, KERNEL_OFFSET
    int 0x13        
    
    jc disk_error   
    
    cmp al, 15      
    jne disk_error
    ret

disk_error:
    mov si, DISK_ERROR_MSG
    call print_string
    jmp $

gdt_start:
    dd 0x0
    dd 0x0
    
    dw 0xffff    
    dw 0x0       
    db 0x0       
    db 10011010b 
    db 11001111b 
    db 0x0       
    
    dw 0xffff    
    dw 0x0       
    db 0x0       
    db 10010010b 
    db 11001111b 
    db 0x0       
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  
    dd gdt_start               

CODE_SEG equ 0x08 
DATA_SEG equ 0x10 

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
    
    call KERNEL_OFFSET
    
    jmp $

KERNEL_OFFSET equ 0x1000
MSG_REAL_MODE db "Booting OS...", 0
DISK_ERROR_MSG db "Disk read error!", 0

times 510-($-$$) db 0
dw 0xaa55 