# Target file name.
TARGET = testkernel

# List C source files here.
CCSOURCES = main.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
NASMSOURCES = start.asm
GASSOURCES = 

# C compiler flags
CFLAGS  = -ggdb -std=gnu99 -fvisibility=hidden -fPIC
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
LDFLAGS = -Map $(ROOT)/bootloader/testkernel/testkernel.map -entry start \
          -nostdlib -T $(ROOT)/bootloader/testkernel/linker.ld

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/testkernel/include $(ROOT)/bootloader/testkernellib/include

# Additional local static libs to link against
LIBS = $(BINARY-testkernellib)

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG -DKERNELAPI_EXPORTS

include $(ROOT)/build/kernel-executable.mak
