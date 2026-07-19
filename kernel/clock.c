#include "clock.h"
#include "port.h"
#include "vga.h"


void pit_init(uint32_t frequency) {
    int divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}


void timer_handler(void) {
    static int tick = 0;
    outb(0x20, 0x20);
    tick++;
    char c = '0' + (tick % 10);
    vga_putchar(c, 0x0F, 0, 0);
}