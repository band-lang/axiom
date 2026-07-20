#include "../include/kernel/port.h"


void outb(const uint16_t port, const uint8_t value) {
  asm volatile("out %0, %1" : : "a"(value), "Nd"(port));
}


uint8_t inb(const uint16_t port) {
  uint8_t result;
  asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}
