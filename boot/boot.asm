bits 32
extern kernel_main


section .multiboot
align 4
    dd 0x1BADB002 ; magic integer
    dd 0x03 ; flags
    dd -(0x1BADB002 + 0x03) ; control sum


section .text
    global _start


_start:
    mov esp, stack_top ; setting stack
    push eax ; save magic integer by grub
    push ebx ; save pointer on structur multiboot
    call kernel_main ; call function from C
    cli ; disable interrupts
    hlt ; stop processor


section .bss
stack_bottom:
    resb 16384 ; reserving 16 KB for stack
stack_top: