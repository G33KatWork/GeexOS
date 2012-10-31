# Target file name.
TARGET = stage15

# List Assembler to be assembled with NASM here
NASMSOURCES = stage15.asm

# NASM flags
NASMFLAGS = -f bin -I$(CURDIR-stage15)/

# Folder for sources
SRCDIR = src

# Folder for object files
OBJDIR = obj

include $(ROOT)/build/nasm-raw.mak

#Extra rule to rebuild stage15 if the embedded stage2 binary changes
$(OBJDIR-stage15)/stage15.bin: $(BINARY-stage2)

#Extra fule to depend on included files
$(OBJDIR-stage15)/stage15.bin: $(SRCDIR-stage15)/a20.inc $(SRCDIR-stage15)/elf.inc $(SRCDIR-stage15)/elfParsing.inc \
	$(SRCDIR-stage15)/gdt.inc $(SRCDIR-stage15)/pe.inc $(SRCDIR-stage15)/peParsing.inc $(SRCDIR-stage15)/pmodePrint.inc 
