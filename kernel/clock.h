#ifndef CLOCK_H
#define CLOCK_H

#include "../include/types.h"

void pit_init(uint32_t frequency);
void timer_handler(void);

#endif