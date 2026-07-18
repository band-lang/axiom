#include "memory.h"


#define HEAP_SIZE (1024 * 1024)


static uint8_t kernel_heap[HEAP_SIZE];
static size_t heap_offset = 0;


// WARNING: Bump allocator, no `free`, no MMU.
// Physical memory is shared between all processes.
// TODO: Implement paging and virtual address spaces.
void *kmalloc(size_t size) {
    if (heap_offset + size > HEAP_SIZE) {
        return 0; // Out of memory.
    }

    void *ptr = &kernel_heap[heap_offset];
    heap_offset += size;
    return ptr;
}