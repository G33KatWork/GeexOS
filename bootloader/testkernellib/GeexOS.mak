# Target file name.
TARGET = testkernellib

# List C source files here.
CCSOURCES = lib.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = 

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
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = -s -Map $(ROOT)/bootloader/testkernellib/testkernellib.map \
          --out-implib $(ROOT)/bootloader/testkernellib/obj/testkernellib_imp.lib \
          -entry _DllEntry --disable-auto-import --exclude-all-symbols \
          -nostdlib -shared --image-base 0xC0000000

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/testkernellib/include

# Additional local static libs to link against
LIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG -DTESTKERNELLIB_EXPORTS

include $(ROOT)/build/kernel-library.mak
