# Target file name.
TARGET = libefi

# List C source files here.
CCSOURCES = boxdraw.c smbios.c console.c \
            crc.c data.c debug.c dpath.c \
            error.c event.c guid.c hand.c \
            hw.c init.c lock.c misc.c \
            print.c sread.c str.c \
            runtime/rtlock.c runtime/efirtlib.c \
            runtime/rtstr.c runtime/vm.c \
            runtime/rtdata.c ia32/initplat.c \
            ia32/math.c


# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS = -ggdb -fpic -Wall -fshort-wchar -fno-strict-aliasing -fno-merge-constants -ffreestanding

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Additional include paths to consider
INCLUDES =	$(ROOT)/bootloader/gnuefi/include \
            $(ROOT)/bootloader/gnuefi/include/$(ARCH) \
            $(ROOT)/bootloader/gnuefi/include/protocol \
            $(ROOT)/bootloader/gnuefi/libefi/include

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/static-lib.mak
