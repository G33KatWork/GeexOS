SRC                 := ../..
include $(SRC)/build/base.mak

# Target file name.
TARGETS = stage1.bin

# List Assembler to be assembled with NASM here
ASOURCES = Stage1.S

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
	$(Q)$(RM) -f stage1.map

distclean: clean

# Assembler S files
%.bin: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean
