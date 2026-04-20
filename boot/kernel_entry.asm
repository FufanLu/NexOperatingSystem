; ============================================================
; Kernel Entry Point
; Bridges from bootloader to C kernel code
; ============================================================

[bits 32]
[extern kernel_main]        ; Defined in kernel.c

; ----- Kernel Entry -----
global _start
_start:
    call kernel_main        ; Call the C kernel
    jmp $                   ; Hang if kernel_main returns
