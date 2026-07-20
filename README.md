# Axiom

Axiom is a small x86_64 (64-bit) kernel written from scratch as a learning project. It boots via GRUB/Multiboot, sets up interrupts, and runs a cooperative-ish scheduler with a handful of processes printing to the VGA text buffer.

This is **not** a production OS — it's a personal project for learning how kernels, interrupts, memory, and processes actually work under the hood.

## Features

- Multiboot-compliant bootloader (`boot/boot.asm`), loaded by GRUB
- VGA text-mode output driver (`kernel/vga.c`)
- GDT-free, IDT-based interrupt handling (`kernel/idt.c`)
- 8259 PIC remapping and IRQ handling (`kernel/pic.c`)
- Programmable Interval Timer (PIT) driver for timer interrupts (`kernel/clock.c`)
- I/O port helpers (`kernel/port.c`)
- Basic process table / process descriptor (`kernel/proc.c`)
- Simple heap allocator (`kernel/memory.c`)

## Planned / in progress

- Virtual memory (paging) for processes
- Inter-process communication (`kernel/ipc.c`)
- Preemptive scheduling driven by the timer interrupt
- Minimal user-space (`user/init.c`, `user/shell.c`, `user/lib.c`)
- Shell

See [`docs/todo.md`](docs/todo.md) and [`docs/architecture.md`](docs/architecture.md) for more detail.

## Project layout

```
Axiom/
├── boot/            # Multiboot entry point (assembly)
├── kernel/          # Kernel C sources and headers
│   └── asm/         # Assembly interrupt stubs
├── include/         # Shared freestanding type definitions
├── user/            # (WIP) user-space programs
├── docs/            # Design notes and TODOs
├── linker.ld        # Linker script (loads kernel at 1MB)
└── Makefile
```

## Requirements

Building and running Axiom requires a 32-bit-capable cross toolchain and an emulator:

- `nasm` — assembler for the boot code and interrupt stubs
- `gcc` with 64-bit support (`-m64`, needs `gcc-multilib` on most distros)
- `ld` with 64-bit ELF support (`binutils`)
- `grub-mkrescue` (from `grub-pc-bin` / `grub2-common`, plus `xorriso`)
- `qemu-system-x86_64` to run the resulting ISO

On Debian/Ubuntu:

```bash
sudo apt install nasm gcc-multilib grub-pc-bin xorriso qemu-system-x86
```

## Building

```bash
make          # builds Axiom.iso
make run      # builds and boots the ISO in QEMU
make debug    # builds and runs QEMU with interrupt logging (-d int)
make debug_gdb # builds and starts QEMU paused, waiting for a GDB connection on :1234
make clean    # removes build artifacts
```

## Debugging with GDB

```bash
make debug_gdb
# in another terminal:
gdb kernel.bin
(gdb) target remote localhost:1234
```

## Status

Axiom is under active, early development and built primarily for learning purposes. Expect missing features, rough edges, and breaking changes.

## License

Axiom is licensed under the MIT License — see [LICENSE.md](LICENSE.md) for details.