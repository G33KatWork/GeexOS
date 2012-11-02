#TODO: this target needs some more love:
#	a propper buildsystem target for efi executables
#	different architectures?
#	warnings
#	objcopy into pe-file if toolchain supports it later
#	objcopy: objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc -j .eh_frame --target=efi-app-ia32 source.so dest.efi

# Target file name.
TARGET = efitest

# List C source files here.
CCSOURCES = main.c

# List C++ source files here.
CXXSOURCES = 

# List Assembler to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS  = -std=gnu99 -ggdb -O0
CFLAGS += -fno-stack-protector -ffreestanding -fPIC -fshort-wchar
CFLAGS += -Wall 

# C++ compiler flags
CXXFLAGS  = 

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = -shared -Bsymbolic -znocombreloc \
          -Map $(ROOT)/bootloader/efitest/efitest.map \
          -nostdlib -T $(ROOT)/bootloader/gnuefi/gnuefi/src/elf_ia32_efi.lds \
          $(ROOT)/bootloader/gnuefi/gnuefi/obj/crt0-efi-ia32.o #horrible hack!

# Additional include paths to consider
INCLUDES = $(ROOT)/bootloader/gnuefi/include \
           $(ROOT)/bootloader/gnuefi/include/ia32 \
           $(ROOT)/bootloader/gnuefi/include/protocol

# Additional local static libs to link against
LIBS = $(BINARY-gnuefi) $(BINARY-libefi)

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/efi-executable.mak
