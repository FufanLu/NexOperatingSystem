# ============================================================
# NexOS Makefile
# Build the bootloader, kernel, and create a bootable image
# ============================================================

# Tools
ASM      = nasm
CC       = i686-elf-gcc
LD       = i686-elf-ld
CFLAGS   = -ffreestanding -m32 -nostdlib -nostdinc -fno-builtin \
           -fno-stack-protector -nostartfiles -nodefaultlibs \
           -Wall -Wextra -I include
LDFLAGS  = -Ttext 0x1000 --oformat binary

# Source files
BOOT_SRC    = boot/boot.asm
ENTRY_SRC   = boot/kernel_entry.asm
KERNEL_SRCS = kernel/kernel.c kernel/shell.c kernel/filesystem.c \
              kernel/string.c drivers/screen.c drivers/keyboard.c

# Object files
ENTRY_OBJ   = build/kernel_entry.o
KERNEL_OBJS = $(patsubst %.c,build/%.o,$(KERNEL_SRCS))

# Output
BOOT_BIN    = build/boot.bin
KERNEL_BIN  = build/kernel.bin
OS_IMAGE    = nexos.img

# ---- Targets ----

all: dirs $(OS_IMAGE)

dirs:
	@mkdir -p build/kernel build/drivers build/boot

# Create final OS image: boot sector + kernel
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	# Pad to floppy disk size (1.44 MB)
	truncate -s 1474560 $(OS_IMAGE)
	@echo "====================================="
	@echo "  NexOS built successfully!"
	@echo "  Image: $(OS_IMAGE)"
	@echo "====================================="

# Assemble bootloader
$(BOOT_BIN): $(BOOT_SRC)
	$(ASM) -f bin $< -o $@

# Assemble kernel entry
$(ENTRY_OBJ): $(ENTRY_SRC)
	$(ASM) -f elf32 $< -o $@

# Compile C source files
build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Run with QEMU
run: all
	qemu-system-i386 -fda $(OS_IMAGE)

# Clean build files
clean:
	rm -rf build/ $(OS_IMAGE)

# Count lines of code
count:
	@echo "Lines of code:"
	@find . -name '*.c' -o -name '*.h' -o -name '*.asm' | \
		xargs wc -l | tail -1

.PHONY: all dirs run clean count
