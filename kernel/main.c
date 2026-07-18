#include "vga.h"
#include "proc.h"


void test_process(void) {
    const char *msg = "Hello from test process";
    vga_clear();
    vga_write(msg, 0x0F, 0, 0);

    while(1);
}


void kernel_main(unsigned long magic, unsigned long addr) {
    const char *msg = "Hello, Kernel.";
    const char *name = "Test";
    vga_clear();
    vga_write(msg, 0x0F, 0, 0);
    proc_create(test_process, name);

    while(1);
}