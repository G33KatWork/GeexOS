# Target file name.
TARGET = stage2

# List C source files here.
CCSOURCES = main.c \
            lib.c \
            print.c \
            debug.c \
            memory.c \
            heap.c \
            disk.c \
            fs/fat.c \
            fs.c \
            loaders/geexos.c \
            exeformats/peloader.c

CCSOURCES += arch/i386/arch.c \
             arch/i386/gdt.c \
             arch/i386/idt.c \
             arch/i386/exception.c \
             arch/i386/serial.c \
             arch/i386/print.c \
             arch/i386/memory.c \
             arch/i386/biosdisk.c \
             arch/i386/mbr_partmap.c \
             arch/i386/math_support.c \
             arch/i386/paging.c \
             arch/i386/gdbstub.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = arch/i386/start.asm \
           arch/i386/trap.asm \
           arch/i386/gdt_flush.asm \
           arch/i386/realmode.asm \
           arch/i386/execute_at.asm

# C compiler flags
CFLAGS  = -ggdb -nostdlib -nostdinc -fno-builtin -std=gnu99 -mno-ms-bitfields
CFLAGS += -fno-stack-check -mno-stack-arg-probe -fno-stack-protector
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
          -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
          -Wstrict-prototypes #-Wconversion

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = -f win32

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = --image-base 0x8000 --subsystem 0x10 -nostdlib -Map $(ROOT)/bootloader/stage2/stage2/stage2.map

# Additional include paths to consider
INCLUDES =	$(ROOT)/bootloader/stage2/stage2/include

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DARCH_I386

include $(ROOT)/build/kernel-executable.mak
