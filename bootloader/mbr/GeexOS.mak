# Target file name.
TARGET = mbr

# List Assembler to be assembled with NASM here
NASMSOURCES = mbr.asm

# NASM flags
NASMFLAGS = -f bin

# Folder for sources
SRCDIR = src

# Folder for object files
OBJDIR = obj

include $(ROOT)/build/nasm-raw.mak
