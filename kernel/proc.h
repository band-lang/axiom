#ifndef PROC_H
#define PROC_H

#include "../include/types.h"

int proc_create(void(*entry)(), const char *name);

typedef enum {
    PROC_RUNNING,
    PROC_READY,
    PROC_BLOCKED,
} process_state_t;


typedef struct {
    int pid;
    char name[32];
    process_state_t state;
    uint32_t eip;
    uint32_t esp;
    uint32_t regs[8];
    uint32_t cr3;
    uint32_t stack;
} process_t;


#endif