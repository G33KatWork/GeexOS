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
            rand.c \
            fs/fat.c \
            fs.c \
            loaders/geexos.c \
            exeformats/loader.c \
            exeformats/elfloader.c

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

# List Assembler to be assembled here
NASMSOURCES = arch/i386/start.asm \
           arch/i386/trap.asm \
           arch/i386/gdt_flush.asm \
           arch/i386/realmode.asm \
           arch/i386/execute_at.asm
GASSOURCES = 

# C compiler flags
CFLAGS  = -ggdb -std=gnu99 -ffreestanding
CFLAGS += -fno-stack-check -mno-stack-arg-probe -fno-stack-protector
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
          -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
          -Wstrict-prototypes #-Wconversion

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = -f elf -ggdb

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = -Map $(ROOT)/bootloader/stage2/stage2/stage2.map -entry start \
          -nostdlib -T $(ROOT)/bootloader/stage2/stage2/linker.ld

# Additional include paths to consider
INCLUDES =	$(ROOT)/bootloader/stage2/stage2/include

# Additional local static libs to link against
LIBS = 

# Shared libraries
SODEPS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DARCH_I386

include $(ROOT)/build/kernel-executable.mak
