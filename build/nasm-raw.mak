#current directory
#FIXME: this looks strange somehow...
CURDIR-$(TARGET) := $(SELF_DIR)$(TARGET)

# Variable mangling
OBJDIR-$(TARGET) := $(addprefix $(CURDIR-$(TARGET))/,$(OBJDIR))
SRCDIR-$(TARGET) := $(addprefix $(CURDIR-$(TARGET))/,$(SRCDIR))

# NASM flags
ASFLAGS-$(TARGET) := $(ASFLAGS)

# Determinte sources
SOURCES-$(TARGET) := $(ASOURCES)

# A name to reference tis target
BINARY-$(TARGET) := $(OBJDIR-$(TARGET))/$(TARGET).bin

# Main targets
all: $(BINARY-$(TARGET))
$(TARGET): $(BINARY-$(TARGET))

$(OBJDIR-$(TARGET))/%.bin: ASFLAGS := $(ASFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/$(TARGET).bin: $(addprefix $(SRCDIR-$(TARGET))/,$(SOURCES-$(TARGET)))
	$(call cmd_msg,NASM,$<)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

# Cleaning
clean: clean-$(TARGET)
clean-$(TARGET): clean-% :
	$(Q)$(RM) -rf $(OBJDIR-$*)

.PHONY: clean-$(TARGET) $(TARGET)
