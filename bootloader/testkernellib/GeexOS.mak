# Target file name.
TARGET = testkernellib

# List C source files here.
CCSOURCES = lib.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS  = -ggdb -std=gnu99 -fvisibility=hidden
CFLAGS += -fPIC
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
          -nostdlib -shared -T $(ROOT)/bootloader/testkernellib/linker.ld

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/testkernellib/include $(ROOT)/bootloader/testkernel/include

# Additional local static libs to link against
LIBS = 

# Shared libraries
SODEPS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG -DTESTKERNELLIB_EXPORTS

include $(ROOT)/build/kernel-library.mak
