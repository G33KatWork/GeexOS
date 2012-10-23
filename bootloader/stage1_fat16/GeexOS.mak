# Target file name.
TARGET = stage1_fat16

# List Assembler to be assembled with NASM here
ASOURCES = stage1.S

# NASM flags
ASFLAGS = -f bin

# Folder for sources
SRCDIR = src

# Folder for object files
OBJDIR = obj

include $(ROOT)/build/nasm-raw.mak
