#current directory
#FIXME: this looks strange somehow...
CURDIR-$(TARGET) := $(SELF_DIR)$(TARGET)

# Variable mangling
OBJDIR-$(TARGET) := $(addprefix $(CURDIR-$(TARGET))/,$(OBJDIR))
SRCDIR-$(TARGET) := $(addprefix $(CURDIR-$(TARGET))/,$(SRCDIR))

# C compiler flags
CFLAGS-$(TARGET) := $(CFLAGS)
CFLAGS-$(TARGET) += $(addprefix -I,$(INCLUDES))
CFLAGS-$(TARGET) += $(DEFINES)

# C++ compiler flags
CXXFLAGS-$(TARGET) := $(CXXFLAGS)
CXXFLAGS-$(TARGET) += $(addprefix -I,$(INCLUDES))
CXXFLAGS-$(TARGET) += $(DEFINES)

# NASM flags
NASMFLAGS-$(TARGET) := $(NASMFLAGS)

# LD flags
LDFLAGS-$(TARGET) := $(LDFLAGS)

# Determinte objects to be created
OBJECTS-$(TARGET) := $(GASSOURCES:%.S=%.o)
OBJECTS-$(TARGET) += $(NASMSOURCES:%.asm=%.o)
OBJECTS-$(TARGET) += $(CCSOURCES:%.c=%.o)
OBJECTS-$(TARGET) += $(CXXSOURCES:%.cpp=%.o)

# Build lib search directories
LIBS-$(TARGET) := $(LIBS)

# Build dependency list of libraries
LIBDEPS-$(TARGET) := $(LIBS)

# A name to reference tis target
BINARY-$(TARGET) := $(OBJDIR-$(TARGET))/$(TARGET).efi

# Main targets
all: $(BINARY-$(TARGET))
$(TARGET): $(BINARY-$(TARGET))

$(OBJDIR-$(TARGET))/$(TARGET).efi: $(OBJDIR-$(TARGET))/$(TARGET).so
	$(call cmd_msg,OBJCOPY,$^ -> $(@))
	$(Q)$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc -j .eh_frame --target=efi-app-ia32 $^ $@

$(OBJDIR-$(TARGET))/$(TARGET).so: LDFLAGS := $(LDFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/$(TARGET).so: $(addprefix $(OBJDIR-$(TARGET))/,$(OBJECTS-$(TARGET))) $(LIBDEPS-$(TARGET))
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^

# Cleaning
clean: clean-$(TARGET)
clean-$(TARGET): clean-% :
	$(Q)$(RM) -f $(CURDIR-$*)/*.map
	$(Q)$(RM) -rf $(OBJDIR-$*)

# Header dependency generation
$(OBJDIR-$(TARGET))/%.d: CFLAGS := $(CFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/%.d: $(SRCDIR-$(TARGET))/%.c
#	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

$(OBJDIR-$(TARGET))/%.d: CXXFLAGS := $(CXXFLAGS-$(TARGET))
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
#	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

# Compile c files
$(OBJDIR-$(TARGET))/%.o: CFLAGS := $(CFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/%.o: $(SRCDIR-$(TARGET))/%.c
	$(call cmd_msg,CC,$<)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Compile cpp files
$(OBJDIR-$(TARGET))/%.o: CXXFLAGS := $(CXXFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/%.o: $(SRCDIR-$(TARGET))/%.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# Assembler asm files with NASM
$(OBJDIR-$(TARGET))/%.o: NASMFLAGS := $(NASMFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/%.o: $(SRCDIR-$(TARGET))/%.asm
	$(call cmd_msg,NASM,$<)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(NASM) $(NASMFLAGS) -o $@ $<

# Assemble S files with GAS
$(OBJDIR-$(TARGET))/%.o: GASFLAGS := $(GASFLAGS-$(TARGET))
$(OBJDIR-$(TARGET))/%.o: $(SRCDIR-$(TARGET))/%.S
	$(call cmd_msg,AS,$<)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(GASFLAGS) -o $@ $<

.PHONY: clean-$(TARGET) $(TARGET)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
ifneq ($(MAKECMDGOALS),clean-$(TARGET))
-include $(addprefix $(OBJDIR-$(TARGET))/, $(OBJECTS-$(TARGET):%.o=%.d))
endif
endif
endif
