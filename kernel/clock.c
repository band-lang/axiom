#include "../include/kernel/clock.h"
#include "../include/kernel/port.h"
#include "../include/kernel/vga.h"


void pit_init(const uint32_t frequency) {
    const uint16_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}


void timer_handler(void) {
    static int tick = 0;
    outb(0x20, 0x20);
    tick++;
    const char c = '0' + (tick % 10); // Dummy
    vga_putchar(c, 0x0F, 0, 0);
}