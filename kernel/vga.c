#include "vga.h"


void vga_clear() {
    char *video = (char*)0xB8000;

    for (int i = 0; i < 80 * 25; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x0F;
    }
}


void vga_putchar(char symbol, uint8_t color, size_t x, size_t y) {
    size_t col = x;
    size_t row = y;
    
    char *video = (char*)0xB8000;
    size_t index = (row * 80 + col) * 2;

    video[index] = symbol; // Symbol
    video[index + 1] = color; // Attribute of color for symbol
}


void vga_write(const char *msg, uint8_t color, size_t x, size_t y) {
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