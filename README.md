# NexOS - A Simple Operating System

A minimal operating system built from scratch using x86 Assembly and C.  
Developed as a course project for NEU MSIS Operating Systems class.

## Features

- **Custom Bootloader**: Written in x86 Assembly, loads kernel from disk
- **Protected Mode**: Transitions from 16-bit Real Mode to 32-bit Protected Mode  
- **VGA Screen Driver**: Direct video memory access for text output with colors
- **PS/2 Keyboard Driver**: Polling-based keyboard input with scancode translation
- **In-Memory Filesystem (NexFS)**: Supports up to 32 files with CRUD operations
- **Interactive Shell**: Command-line interface with 12 built-in commands

## Implemented Commands (Functions)

| Command | Alias | Description |
|---------|-------|-------------|
| `list` | `ls` | List all files with sizes |
| `read` | `cat` | Display file contents |
| `create` | `touch` | Create a new file |
| `delete` | `rm` | Delete a file |
| `rename` | `mv` | Rename a file |
| `copy` | `cp` | Copy a file |
| `write` | - | Write content to a file |
| `clear` | `cls` | Clear the screen |
| `info` | - | Show system information |
| `echo` | - | Print text to screen |
| `about` | - | About NexOS |
| `help` | - | Show command reference |

## Building

### Prerequisites
- NASM (Netwide Assembler)
- i686-elf cross-compiler (GCC)
- QEMU (for testing)

### Build & Run
```bash
make            # Build the OS image
make run        # Build and run in QEMU
make clean      # Clean build files
make count      # Count lines of code
```

## Project Structure
```
myos/
├── boot/
│   ├── boot.asm           # Bootloader (Real Mode → Protected Mode)
│   └── kernel_entry.asm   # Kernel entry point (ASM → C bridge)
├── drivers/
│   ├── screen.c           # VGA text-mode screen driver
│   └── keyboard.c         # PS/2 keyboard driver
├── kernel/
│   ├── kernel.c           # Kernel main entry
│   ├── shell.c            # Interactive shell with commands
│   ├── filesystem.c       # In-memory filesystem
│   └── string.c           # String utility functions
├── include/
│   ├── screen.h
│   ├── keyboard.h
│   ├── filesystem.h
│   ├── string.h
│   ├── ports.h
│   └── shell.h
├── Makefile
└── README.md
```

## Architecture

```
Bootloader (boot.asm)
    ↓ Loads kernel sectors
    ↓ Sets up GDT
    ↓ Switches to Protected Mode
Kernel Entry (kernel_entry.asm)
    ↓ Calls kernel_main()
Kernel (kernel.c)
    ↓ Initializes screen driver
    ↓ Initializes filesystem
    ↓ Starts shell
Shell (shell.c)
    ↓ Read input → Parse → Execute → Loop
```

## Author

Fufan Lu  
Northeastern University, Seattle  
