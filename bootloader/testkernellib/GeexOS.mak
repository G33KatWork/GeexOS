# Target file name.
TARGET = testkernellib

# List C source files here.
CCSOURCES = lib.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = 

# C compiler flags
CFLAGS  = -ggdb -nostdlib -nostdinc -fno-builtin -std=gnu99 -fvisibility=hidden -fPIC
CFLAGS += -fno-stack-check -mno-stack-arg-probe -fno-stack-protector
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
          -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
          -Wstrict-prototypes #-Wconversion

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = -Map $(ROOT)/bootloader/testkernellib/testkernellib.map \
          -entry DllEntry -nostdlib -shared

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/testkernellib/include $(ROOT)/bootloader/testkernel/include

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG -DTESTKERNELLIB_EXPORTS

include $(ROOT)/build/kernel-library.mak
