SRC                 := ../..
include $(SRC)/build/base.mak

# Target file name.
TARGETS = mbr.bin

# List Assembler to be assembled with NASM here
ASOURCES = mbr.S

# Additional include paths to consider
INCLUDES =	include

# Folder for sourcecode
SRCDIR = src

# NASM flags
ASFLAGS = -f bin

# Main targets
all: $(TARGETS)
	@:	#Shut up!

$(TARGETS): $(addprefix $(SRCDIR)/, $(OBJECTS))

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGETS)

# Assembler S files
%.bin: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean
