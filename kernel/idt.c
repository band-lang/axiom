#include "../include/kernel/idt.h"


static idt_entry_t idt[256];


void idt_init(void) {
    struct {
        uint16_t limit;
        uint64_t base;
    }__attribute__((packed)) idtr;

    idtr.limit = (256 * sizeof(idt_entry_t) - 1); // Limit of size structure
    idtr.base = (uintptr_t)&idt; // Pointer on first byte in IDT

    asm volatile("lidt %0" : : "m"(idtr));
}


void idt_set_gate(const int n, const uintptr_t handler, const uint16_t flags) {
    idt[n].base_low = handler & 0xFFFF; // Low 16 bits of address
    idt[n].base_mid = (handler >> 16) & 0xFFFF; // Mid 16 bits of address
    idt[n].base_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].selector = 0x08; // Segment of code in Ring0
    idt[n].reserved = 0; // Dummy for dummy CPU
    idt[n].flags = flags; // Flags (0x8E), Present | DPL = 0 | Type = 0xE, 32 bits interrupts
    idt[n].reserved2 = 0;
}