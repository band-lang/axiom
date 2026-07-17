#include "vga.h"


void kernel_main(unsigned long magic, unsigned long addr) {
    const char *msg = "Hello, Kernel.";
    vga_clear();
    vga_write(msg, 0x0F, 0, 0);

    while(1);
}