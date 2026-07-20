# Compilers and flags
NASM = nasm
GCC = gcc
LD = ld

NASM_FLAGS = -f elf64
GCC_FLAGS = -m64 -c -ffreestanding -nostdlib -Iinclude -Ikernel -g
LD_FLAGS = -m elf_x86_64 -T linker.ld -nostdlib

# List of object files of core
KERNEL_OBJS = boot/boot.o \
              kernel/main.o \
              kernel/vga.o \
              kernel/proc.o \
              kernel/ipc.o \
              kernel/interrupt.o \
              kernel/memory.o \
              kernel/clock.o \
			  kernel/idt.o \
			  kernel/pic.o \
			  kernel/port.o \
			  kernel/asm/interrupt.o

# Default ultimate goal
all: Axiom.iso

# Asm loads
boot/boot.o: boot/boot.asm
	$(NASM) $(NASM_FLAGS) boot/boot.asm -o boot/boot.o

kernel/asm/%.o: kernel/asm/%.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

# All C-Files of kernel
kernel/%.o: kernel/%.c
	$(GCC) $(GCC_FLAGS) $< -o $@

# Linkd of core
kernel.bin: $(KERNEL_OBJS)
	$(LD) $(LD_FLAGS) -o kernel.bin $(KERNEL_OBJS)

# Create iso
Axiom.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Axiom" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o Axiom.iso iso/

# Running
run: Axiom.iso
	qemu-system-x86_64 -cdrom Axiom.iso

#Debug
debug: Axiom.iso
	qemu-system-x86_64 -cdrom Axiom.iso -d int -no-reboot -no-shutdown


#Debug gdb
debug_gdb: Axiom.iso
	qemu-system-x86_64 -cdrom Axiom.iso -s -S

# Clean
clean:
	rm -rf boot/*.o kernel/*.o kernel/asm/*.o kernel.bin Axiom.iso iso