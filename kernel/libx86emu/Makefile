SRC                 := ../..
include $(SRC)/build/base.mak

# Target file name.
TARGET = x86emu

# List C source files here.
CCSOURCES = 

# List C++ source files here
CXXSOURCES = x86Emulator.cpp \
			 x86EmulatorInstructionDecoder.cpp \
			 x86EmulatorMemory.cpp \
			 x86Registers.cpp \
			 main.cpp

# List Assembler to be assembled with NASM here
ASOURCES = 

# Additional include paths to consider
INCLUDES =	include

# Folder for object files
OBJDIR = objhost

# Folder for sourcecode
SRCDIR = src

# C compiler flags
CFLAGS += -ggdb -std=gnu99
CFLAGS += -DDEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -ggdb
CXXFLAGS += -DDEBUG
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# NASM flags
ASFLAGS = -f elf -ggdb

# Determinte objects to be created
OBJECTS += $(ASOURCES:%.S=%.o)
OBJECTS += $(GASOURCES:%.s=%.o)
OBJECTS += $(CXXSOURCES:%.cpp=%.o)
OBJECTS += $(CCSOURCES:%.c=%.o)

# define output-directories
# sort for removing duplicates
OBJDIRS = $(sort $(addprefix $(OBJDIR)/, $(dir $(OBJECTS))))

# Main targets
all: createdirs $(TARGET)
	@:	#Shut up!

# Create output directories
$(OBJDIRS):
	$(call cmd_msg,MKDIR,$(@))
	$(Q)$(MKDIR) -p $@

createdirs: $(OBJDIRS)

$(TARGET): $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(call cmd_msg,LD,$(@))
	$(Q)$(HOSTCXX) -o $@ $^

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGET)
	$(Q)$(RM) -rf $(OBJDIR)

# Header dependency generation
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(HOSTCXX) $(CXXFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

$(OBJDIR)/%.d: $(SRCDIR)/%.c
	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(HOSTCC) $(CFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

# Compile cpp files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(HOSTCXX) $(CXXFLAGS) -c $< -o $@

# Compile c files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(call cmd_msg,CC,$<)
	$(Q)$(HOSTCC) $(CFLAGS) -c $< -o $@

# Assembler S files
$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: createdirs clean

ifneq ($(MAKECMDGOALS),clean)
-include $(addprefix $(OBJDIR)/, $(OBJECTS:%.o=%.d))
endif

