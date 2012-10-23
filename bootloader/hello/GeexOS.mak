# Target file name.
TARGET = hello

# List Assembler to be assembled with NASM here
ASOURCES = hello.S

# NASM flags
ASFLAGS = -f bin

# Folder for sources
SRCDIR = src

# Folder for object files
OBJDIR = obj

include $(ROOT)/build/nasm-raw.mak
