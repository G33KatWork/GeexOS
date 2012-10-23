# Target file name.
TARGET = testkernel

# List C source files here.
CCSOURCES = main.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = start.S

# C compiler flags
CFLAGS  = -ggdb -nostdlib -nostdinc -fno-builtin -std=gnu99
CFLAGS += -fno-stack-check -mno-stack-arg-probe -fno-stack-protector
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
          -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
          -Wstrict-prototypes #-Wconversion

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
ASFLAGS = -ggdb -f win32

# Linker flags
LDFLAGS = -Map $(ROOT)/kernel/kernel/testkernel.map -entry _start \
          --image-base 0xC0000000 --subsystem native --disable-auto-import \
          --exclude-all-symbols -nostdlib -pie

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/testkernellib/include

# Additional local static libs to link against
LIBS = $(IMPLIB-testkernellib)

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG

include $(ROOT)/build/kernel-executable.mak
