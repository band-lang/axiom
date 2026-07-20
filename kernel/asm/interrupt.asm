extern timer_handler


section .text
    global timer_handler_asm


timer_handler_asm:
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11
    push rax
    push rbx
    push rbp

    call timer_handler

    pop rbp
    pop rbx
    pop rax
    pop r11
    pop r10
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    iretq