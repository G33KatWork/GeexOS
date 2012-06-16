SRC                 := ../../..
include $(SRC)/build/base.mak

# Target file name.
TARGET = stage2.exe

# List C source files here.
CCSOURCES = main.c \
			lib.c \
			print.c \
			debug.c \
			memory.c \
			heap.c \
			disk.c \
			fs/fat.c \
			fs.c \
			loaders/geexos.c \
			exeformats/peloader.c

CCSOURCES += arch/i386/arch.c \
			 arch/i386/gdt.c \
			 arch/i386/idt.c \
			 arch/i386/exception.c \
			 arch/i386/serial.c \
			 arch/i386/print.c \
			 arch/i386/memory.c \
			 arch/i386/biosdisk.c \
			 arch/i386/mbr_partmap.c \
			 arch/i386/math_support.c \
			 arch/i386/paging.c \
			 arch/i386/gdbstub.c

# List C++ source files here
CXXSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = arch/i386/start.S \
		   arch/i386/trap.S \
		   arch/i386/gdt_flush.S \
		   arch/i386/realmode.S \
		   arch/i386/execute_at.S

# Additional include paths to consider
INCLUDES =	include

# Additional static libs to link agains
STATICLIBS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# C compiler flags
CFLAGS += -ggdb -nostdlib -nostdinc -fno-builtin -std=gnu99
#TODO: add library support for security features below
CFLAGS += -fno-stack-check -mno-stack-arg-probe -fno-stack-protector
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wstrict-prototypes #-Wconversion
CFLAGS += -DARCH_I386 -DDEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wredundant-decls -Winline -Wno-long-long
CXXFLAGS += -DARCH_I386 -DDEBUG
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# NASM flags
ASFLAGS = -f win32 -ggdb

# Linker flags
LDFLAGS = -Map stage2.map --image-base 0x8000 --subsystem 0x10 -nostdlib
#uncomment following line to strip binary
#LDFLAGS += -s

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

$(TARGET): $(addprefix $(OBJDIR)/, $(OBJECTS)) $(STATICLIBS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(STATICLIBS)
	$(call cmd_msg,DISASM,$(@) -> disasm)
	$(Q)$(OBJDUMP) -d $(TARGET) > disasm

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGET)
#$(Q)$(RM) -f $(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)$(RM) -f disasm
	$(Q)$(RM) -f stage2.map
	$(Q)$(RM) -rf obj
	$(Q)$(RM) -rf lib$(TARGET).a

distclean: clean

# Header dependency generation
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

$(OBJDIR)/%.d: $(SRCDIR)/%.c
	$(call cmd_msg,DEPENDS,$@)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -MM -MG -MP -MT '$(@:%.d=%.o)' $< > $@

# Compile cpp files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

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
