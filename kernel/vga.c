#include "../include/kernel/vga.h"


void vga_clear() {
    char *video = (char*)0xB8000;

    for (int i = 0; i < 80 * 25; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x0F;
    }
}


void vga_putchar(const char symbol, const uint8_t color, const size_t x, const size_t y) {
    const size_t col = x;
    const size_t row = y;
    
    char *video = (char*)0xB8000;
    const size_t index = (row * 80 + col) * 2;

    video[index] = symbol; // Symbol
    video[index + 1] = (char)color; // Attribute of color for symbol
}


void vga_write(const char *msg, const uint8_t color, const size_t x, const size_t y) {
    size_t col = x;
    size_t row = y;

    char *video = (char*)0xB8000;

    int i = 0;
    while (msg[i] != '\0') {
        vga_putchar(msg[i], color, col, row);
        col++;
        if (row >= 80) {
            row = 0;
            col++;
        }
        i++;
    }
}