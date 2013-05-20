# Target file name.
TARGET = gnuefi

# List C source files here.
CCSOURCES = reloc_$(ARCH).c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
NASMSOURCES = 
GASSOURCES = crt0-efi-$(ARCH).S

# C compiler flags
CFLAGS = -ggdb -fpic -Wall -fshort-wchar -fno-strict-aliasing -fno-merge-constants -ffreestanding

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Additional include paths to consider
INCLUDES =	$(ROOT)/bootloader/gnuefi/include $(ROOT)/bootloader/gnuefi/include/$(ARCH)

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/static-lib.mak
