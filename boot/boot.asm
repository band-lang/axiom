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
    ; Save magic and multiboot_info from GRUB
    mov esi, eax
    mov edi, ebx

    ; Check for support CPUID
    pushfd ; read eflags from processor and push values in stack
    pop eax ; take out eflags value from head of stack and save to eax
    mov ecx, eax ; save copy of flags to ecx
    xor eax, 1 << 21 ; invert 21st bit (0 -> 1, 1 -> 0) to test if CPUID is supported
    push eax ; save value from eax to stack
    popfd ; reading new value from stack and save to eflags
    pushfd ; read eflags to stack
    pop eax ; take out eax from stack
    xor eax, ecx ; if 0 - 21 bit dont changed, processor dont have cpuid
    jz no_cpuid ; jump to no_cpuid point

    ; Check for support long mode
    mov eax, 0x80000000 ; Ask a question to processor about supporting long mode
    cpuid ; Calls processor to get answer
    cmp eax, 0x80000001 ; if maximum value lower then 0x80000001 long mode is not supported
    jb no_longmode ; jump if value is lower
    mov eax, 0x80000001 ; Ask a question to processor to get more info about processor
    cpuid ; Calls processor to get answer
    test edx, 1 << 29 ; From eax get info about processor, then get ZF or nothing
    jz no_longmode ; If ZF is true then processor dont have longmode

    ; Fill in PML4[0] = Address PDP + flags (Present=1, Read/Write=1)
    mov eax, pdp_table ; Address of pdp_table
    or eax, 0x3 ; Present + R/W
    mov [pml4_table], eax ; Fill value from eax into plm_table memory

    ; Fill in PDP[0] = Address PD + flags
    mov eax, pd_table ; Address of pd_table
    or eax, 0x3
    mov [pdp_table], eax

    ; Fill PD[0] = 0 (physical address 0) + flags (Present, R/W, Page Size=2MB)
    mov eax, 0x0 ; Start of physical memory
    or eax, 0x83 ; Present + R/W + Page Size (bit 7)
    mov [pd_table], eax

    mov eax, 0x200000
    or eax, 0x83
    mov [pd_table + 8], eax

    ; Activating mode PAE
    mov eax, cr4 ; Moving value from cr4 to eax
    or eax, 1 << 5 ; If 5 bit = 0, then 5 bit = 1
    mov cr4, eax ; Moving value from eax to cr4

    ; Load address of pml_table header to cr3
    mov eax, pml4_table
    mov cr3, eax

    ; Enable long mode in EFER MSR
    mov ecx, 0xC0000080 ; Address of EFER MSR
    rdmsr ; Read current value edx:eax
    or eax, 1 << 8 ; Bit 8 = LME (Long mode enabled)
    wrmsr

    ; Enable Paging
    mov eax, cr0 ; Moving value from cr0 to eax
    or eax, 1 << 31 ; Bit 31 = PG(Paging)
    mov cr0, eax

    ; Load gdt 64 bits
    lgdt [gdt64_ptr]

    jmp 0x08:long_mode_start


[bits 64]
long_mode_start:
    ; Setting registers of segments
    mov ax, 0x10 ; Selector of data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Setting stack
    mov rsp, stack_top

    ; Setting vars for arguments of func kernel_main
    mov eax, edi      ; eax = multiboot_info (template)
    mov edi, esi      ; rdi = magic (first argument)
    mov esi, eax      ; rsi = multiboot_info (second argument)

    push rdi
    push rsi

    call kernel_main ; Call kernel_main function

    cli ; Stop interrupts
    htl ; Stop processor


no_cpuid:
    mov dword [0xB8000], 0x4F4F4F4F ; NO CPUID in VGA
    cli ; disable interrupts
    hlt ; stop processor


no_longmode:
    mov dword [0xB8000], 0x4F4F4F4F ; NO LONG in VGA
    cli ; disable interrupts
    hlt ; stop processor


section .data
    gdt64:
        dq 0                    ; Zero filling
        dq 0x0020980000000000   ; Segment of code (64 bit)
        dq 0x0000920000000000   ; Segment of data
    gdt64_ptr:
        dw $ - gdt64 - 1        ; Size of GDT - 4
        dd gdt64                 ; Address of GDT


section .bss
    align 4096
        pml4_table: resb 4096 ; Highest table
        pdp_table: resb 4096 ; Middle table
        pd_table: resb 4096 ; Lowest table
    stack_bottom:
        resb 16384 ; reserving 16 KB for stack
    stack_top: