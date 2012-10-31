# Target file name.
TARGET = stage15

# List Assembler to be assembled with NASM here
ASOURCES = stage15.asm

# NASM flags
NASMFLAGS = -f bin -I$(CURDIR-stage15)/

# Folder for sources
SRCDIR = src

# Folder for object files
OBJDIR = obj

include $(ROOT)/build/nasm-raw.mak

#Extra rule to rebuild stage15 if the embedded stage2 binary changes
$(OBJDIR-stage15)/stage15.bin: $(BINARY-stage2)
