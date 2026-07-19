#include "port.h"


void pic_init(void) {
    // ICW1: Start initialize, use ICW4
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: New offset of vectors
    outb(0x21, 0x20); // Master PIC: vectors 32-39
    outb(0xA1, 0x28); // Slave PIC: vectors 40-47

    // ICW3: Relationship between PIC
    outb(0x21, 0x04); // Master PCI connected to IRQ 2
    outb(0xA1, 0x02); // Slave PCI connected to IEQ 2 master

    // ICW4: Mode x86, not buffered, normal EOI
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}