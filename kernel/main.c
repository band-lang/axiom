#include "../include/kernel/vga.h"
#include "../include/kernel/idt.h"
#include "../include/kernel/pic.h"
#include "../include/kernel/clock.h"
#include "../include/kernel/port.h"


// Declare function from asm
extern void timer_handler_asm(void);


// Garbage
static void test_process(void) {
    const char *msg = "Hello from test process";
    vga_clear();
    vga_write(msg, 0x0F, 0, 0);

    while(1);
}


void kernel_main(unsigned long magic, unsigned long multiboot_info) {
    if (magic != 0x2BADB002) {
        vga_clear();
        char *error_msg = "Error: not loaded by Multiboot-compliant bootloader!";
        vga_write(error_msg, 0x0C, 9, 12);
        while(1);
    }

    vga_clear();

    // 1. Initialization
    idt_init(); // Load IDT to processor
    pic_init(); // Setting PIC (IRQ - vectors)

    // 2. Full in IDT for timer
    idt_set_gate(32, (uintptr_t)timer_handler_asm, 0x8E); // IDT[32] -> time_handler

    // 3. Start timer
    pit_init(100);

    // Unmask IRQ 0
    outb(0x21, 0xFE);
    outb(0xA1, 0xFE);

    // 5. Allow interrupts
    asm volatile("sti");

    // 6. Wating for interrupts
    while(1);
}