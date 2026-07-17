#ifndef VGA_H
#define VGA_H


#include "../include/types.h"


void vga_clear(void);
void vga_putchar(char c, uint8_t color, size_t x, size_t y);
void vga_write(const char *msg, uint8_t color, size_t x, size_t y);


#endif