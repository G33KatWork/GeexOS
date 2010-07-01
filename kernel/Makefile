SRC                 := ..
include $(SRC)/build/base.mak

# Target file name.
TARGET = kernel.elf

# List C source files here.
CCSOURCES = 

# List C++ source files here
CXXSOURCES = lib/string.cpp \
             kernel/main.cpp \
             kernel/IO/CharacterOutputDevice.cpp \
			 kernel/IO/Monitor.cpp \
			 kernel/IO/SerialConsole.cpp \
			 kernel/global.cpp \
			 lib/cppruntime.cpp \
			 kernel/utils/multiboot.cpp \
			 arch/Paging.cpp \
			 arch/gdt.cpp \
			 arch/idt.cpp \
			 arch/pic.cpp  \
			 arch/pit.cpp  \
			 arch/hal.cpp  \
			 arch/APIC.cpp \
			 arch/ACPI/RSDP.cpp \
			 arch/ACPI/RSDT.cpp \
			 arch/ACPI/ACPITable.cpp \
			 arch/interrupts.cpp \
			 kernel/Processes/Scheduler.cpp \
			 kernel/Processes/Thread.cpp \
			 kernel/DataStructures/OrderedArray.cpp \
			 arch/PageFaultHandler.cpp \
			 kernel/DataStructures/List.cpp\
			 kernel/Time/Timer.cpp\
			 kernel/Time/TimerManager.cpp \
 			 kernel/Memory/PlacementAllocator.cpp \
             kernel/Memory/Virtual/VirtualMemoryManager.cpp \
             kernel/Memory/Virtual/VirtualMemorySpace.cpp \
             kernel/Memory/Virtual/VirtualMemoryRegion.cpp \
			 kernel/Memory/BitfieldPhysicalMemoryManager.cpp \
			 kernel/Memory/IO/IOMemoryManager.cpp \
			 kernel/utils/ElfInformation.cpp \
			 kernel/Processes/Scheduler.cpp \
			 kernel/Processes/Thread.cpp \
			 kernel/Processes/KernelThread.cpp \
			 kernel/Memory/Virtual/Regions/KernelStackMemoryRegion.cpp \
			 kernel/IO/CharacterInputDevice.cpp
#			 kernel/Memory/Slab/SlabManager.cpp

# List Assembler to be assembled with NASM here
ASOURCES = arch/start.S \
		   arch/interrupts_stubs.S \
		   arch/scheduling.S \
		   arch/gdt_flush.S

# Additional include paths to consider
INCLUDES =	include

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
CFLAGS += -DARCH_X86 -DDEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wredundant-decls -Winline -Wno-long-long \
			-Wconversion #-Werror
CXXFLAGS += -DARCH_X86 -DDEBUG
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# NASM flags
ASFLAGS = -f elf -ggdb

# Linker flags
LDFLAGS = -T linker.ld -Map kernel.map

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
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(call cmd_msg,DISASM,$(@) -> disasm)
	$(Q)$(OBJDUMP) -d $(TARGET) > disasm

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGET)
#$(Q)$(RM) -f $(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)$(RM) -f disasm
	$(Q)$(RM) -f kernel.map
	$(Q)$(RM) -rf obj

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
-include $(addprefix $(OBJDIR)/, $(OBJECTS:%.o=%.d))
endif
