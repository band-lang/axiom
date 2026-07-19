# Architecture Notes

## Boot flow

1. GRUB loads `kernel.bin` as a Multiboot kernel and jumps to `_start` in `boot/boot.asm`.
2. `_start` sets up a small stack and calls `kernel_main(magic, multiboot_addr)` in `kernel/main.c`.
3. `kernel_main` initializes the VGA driver, IDT, and PIC, installs the timer interrupt handler, starts the PIT, unmasks IRQ0, and enables interrupts (`sti`).
4. The kernel then idles in a `while(1)` loop, servicing interrupts as they arrive.

## Interrupts

- `kernel/idt.c` builds the 256-entry Interrupt Descriptor Table and exposes `idt_set_gate` to register handlers.
- `kernel/pic.c` remaps the 8259 PIC so hardware IRQs land at vectors 32+ instead of colliding with CPU exceptions.
- `kernel/clock.c` programs the PIT (channel 0) to fire at a configurable frequency, driving `timer_handler`.
- Low-level interrupt entry/exit stubs live in `kernel/asm/interrupt.asm`, which save/restore CPU state before calling into C.

## Processes

- `kernel/proc.h` defines `process_t`: a PID, name, state (`PROC_RUNNING` / `PROC_READY` / `PROC_BLOCKED`), saved registers, stack pointer, and `cr3` (page directory, for future paging support).
- `kernel/proc.c` currently provides `proc_create` to register a new process entry; scheduling and context switching are not implemented yet.

## Memory

- `kernel/memory.c` provides a minimal `kmalloc` for early kernel allocations.
- No virtual memory / paging yet — this is the next major piece of work (see `todo.md`).

## Planned: IPC and user space

- `kernel/ipc.{c,h}` and the `user/` directory are stubs reserved for future work: message-passing IPC and a minimal user-mode init/shell.
- task sheduler