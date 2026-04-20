; ============================================================
; NexOS Bootloader
; A simple bootloader that sets up the environment and
; loads the kernel into memory.
; ============================================================

[org 0x7c00]            ; BIOS loads bootloader at 0x7c00
[bits 16]               ; Start in 16-bit real mode

KERNEL_OFFSET equ 0x1000  ; Memory offset where kernel is loaded

; ----- Entry Point -----
boot_start:
    mov [BOOT_DRIVE], dl   ; BIOS stores boot drive number in dl

    ; Set up the stack
    mov bp, 0x9000
    mov sp, bp

    ; Print boot message
    mov bx, MSG_BOOT
    call print_string_rm

    ; Load kernel from disk
    call load_kernel

    ; Switch to 32-bit protected mode
    call switch_to_pm

    jmp $                  ; Should never reach here

; ----- Print string in Real Mode -----
; Expects: bx = pointer to null-terminated string
print_string_rm:
    pusha
    mov ah, 0x0e           ; BIOS teletype function
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    int 0x10
    inc bx
    jmp .loop
.done:
    popa
    ret

; ----- Load kernel sectors from disk -----
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string_rm

    mov bx, KERNEL_OFFSET  ; Read to KERNEL_OFFSET
    mov dh, 15             ; Read 15 sectors
    mov dl, [BOOT_DRIVE]

    ; BIOS disk read interrupt
    mov ah, 0x02           ; Read sectors function
    mov al, dh             ; Number of sectors
    mov ch, 0              ; Cylinder 0
    mov cl, 2              ; Start from sector 2 (sector 1 = boot)
    mov dh, 0              ; Head 0
    int 0x13
    jc disk_error          ; Jump if carry flag set (error)

    ret

disk_error:
    mov bx, MSG_DISK_ERR
    call print_string_rm
    jmp $

; ----- GDT (Global Descriptor Table) -----
gdt_start:
    ; Null descriptor (required)
    dd 0x0
    dd 0x0

gdt_code:
    ; Code segment descriptor
    dw 0xffff              ; Limit (bits 0-15)
    dw 0x0                 ; Base (bits 0-15)
    db 0x0                 ; Base (bits 16-23)
    db 10011010b           ; Access byte
    db 11001111b           ; Flags + Limit (bits 16-19)
    db 0x0                 ; Base (bits 24-31)

gdt_data:
    ; Data segment descriptor
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size
    dd gdt_start                 ; GDT address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; ----- Switch to 32-bit Protected Mode -----
switch_to_pm:
    cli                    ; Disable interrupts
    lgdt [gdt_descriptor]  ; Load GDT

    ; Set PE (Protection Enable) bit in CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Far jump to flush pipeline and enter 32-bit mode
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000      ; Set up stack in protected mode
    mov esp, ebp

    ; Print success message
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    ; Jump to kernel
    call KERNEL_OFFSET

    jmp $                  ; Hang if kernel returns

; ----- Print string in Protected Mode -----
; Writes directly to video memory at 0xb8000
print_string_pm:
    pusha
    mov edx, 0xb8000       ; Video memory address
.loop:
    mov al, [ebx]
    mov ah, 0x0f            ; White on black attribute
    cmp al, 0
    je .done
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp .loop
.done:
    popa
    ret

; ----- Data -----
BOOT_DRIVE:     db 0
MSG_BOOT:       db "NexOS Bootloader v1.0", 13, 10, 0
MSG_LOAD_KERNEL: db "Loading kernel...", 13, 10, 0
MSG_DISK_ERR:   db "Disk read error!", 13, 10, 0
MSG_PROT_MODE:  db "Entered 32-bit Protected Mode", 0

; ----- Boot Sector Padding -----
times 510-($-$$) db 0     ; Pad to 510 bytes
dw 0xaa55                  ; Boot signature
