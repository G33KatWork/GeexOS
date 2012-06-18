SRC                 := ../..
include $(SRC)/build/base.mak

# Target file name.
TARGET = testkernellib.dll

# List C source files here.
CCSOURCES = lib.c

# List C++ source files here
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = 

# Additional include paths to consider
INCLUDES =	include

# Additional static libs to link agains
STATICLIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# C compiler flags
CFLAGS += -ggdb -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=gnu99
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wstrict-prototypes #-Wconversion
CFLAGS += -DARCH_X86 -DDEBUG -DTESTKERNELLIB_EXPORTS
CFLAGS += $(addprefix -I,$(INCLUDES))

# NASM flags
ASFLAGS = -f win32 -ggdb

# Linker flags
LDFLAGS = -s -Map testkernellib.map --out-implib testkernellib_imp.a \
			-entry _DllEntry --disable-auto-import --exclude-all-symbols \
			-nostdlib -shared --image-base 0xC0000000

# Determinte objects to be created
OBJECTS += $(ASOURCES:%.S=%.o)
OBJECTS += $(GASOURCES:%.s=%.o)
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

$(TARGET): $(addprefix $(OBJDIR)/, $(OBJECTS)) $(STATICLIBS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(STATICLIBS)
	$(call cmd_msg,DISASM,$(@) -> disasm)
	$(Q)$(OBJDUMP) -d $(TARGET) > disasm

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGET) testkernellib_imp.a
	$(Q)$(RM) -f disasm
	$(Q)$(RM) -f testkernellib.map
	$(Q)$(RM) -rf obj

distclean: clean

# Header dependency generation
$(OBJDIR)/%.d: $(SRCDIR)/%.c
	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

# Compile c files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(call cmd_msg,CC,$<)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Assembler S files
$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: createdirs clean

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(addprefix $(OBJDIR)/, $(OBJECTS:%.o=%.d))
endif
endif
