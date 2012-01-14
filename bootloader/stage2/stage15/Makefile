SRC                 := ../../..
include $(SRC)/build/base.mak

# Target file name.
TARGETS = stage15.bin

# List Assembler to be assembled with NASM here
ASOURCES = stage15.S

# List of assembler includes
ASDEPENDS = a20.inc gdt.inc pmodePrint.inc

# Additional include paths to consider
INCLUDES =	include

# Folder for sourcecode
SRCDIR = src

# NASM flags
ASFLAGS = -f bin

# Main targets
all: $(TARGETS)
	@:	#Shut up!

$(TARGETS): $(addprefix $(SRCDIR)/, $(OBJECTS)) $(addprefix $(SRCDIR)/, $(ASDEPENDS)) ../stage2/stage2.exe

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGETS)
	$(Q)$(RM) -f stage15.map

distclean: clean

# Assembler S files
%.bin: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean
