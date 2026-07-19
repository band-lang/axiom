#include "proc.h"
#include "memory.h"


#define MAX_PROCCESSES 256
#define DEFAULT_STACK_SIZE 8192


static process_t process_table[MAX_PROCCESSES];
static uint32_t next_pid = 1;


int proc_create(void(*entry)(), const char* name) {
    int slot = -1;
    for (int i = 0; i < MAX_PROCCESSES; i++) {
        if (process_table[i].pid == 0) {
            slot = i;
            break;
        }
    }

    if (slot == -1) return -1;

    process_t *proc = &process_table[slot];
    void *stack_memory = kmalloc(DEFAULT_STACK_SIZE);

    proc->pid = next_pid;
    proc->state = PROC_READY;
    proc->eip = (uint32_t)entry;
    proc->esp = (uint32_t)(stack_memory + DEFAULT_STACK_SIZE);
    proc->stack = (uint32_t)stack_memory;
    proc->cr3 = (uint32_t)0; // dummy

    int i = 0;
    for (i = 0; i < 32 && name[i] != '\0'; i++) {
        proc->name[i] = name[i];
    }
    proc->name[i] = '\0';

    for (int r = 0; r < sizeof(proc->regs) / sizeof(proc->regs[0]); r++) {
        proc->regs[r] = 0;
    }

    return proc->pid;
}