#ifndef IDT_H
#define IDT_H

#include "../include/types.h"

// Structs
typedef struct {
    uint16_t base_low; // Low 16 bits of handler address
    uint16_t selector; // Code segment selector (0x08 for kernel - Ring0)
    uint8_t reserved; // Always zero. Reserved
    uint8_t flags; // Flags: Present (0x80) | DPL (0x00=Ring0) | Type (0x0E=32-bit interrupt)
    uint16_t base_high; // High 16 bits of handler address
} __attribute__((packed)) idt_entry_t;

// Functions
void idt_init(void);
void idt_set_gate(int n, uint32_t handler, uint16_t flags);

#endif