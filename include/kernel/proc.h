#ifndef PROC_H
#define PROC_H

#include "../types.h"

uint32_t proc_create(void(*entry)(), const char *name);

typedef enum {
    PROC_RUNNING,
    PROC_READY,
    PROC_BLOCKED,
} process_state_t;


typedef struct {
    uint32_t pid;
    char name[32];
    process_state_t state;
    uint64_t eip;
    uint64_t esp;
    uint64_t regs[16];
    uint64_t cr3;
    uint64_t stack;
} process_t;


#endif