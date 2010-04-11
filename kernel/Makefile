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
			 kernel/utils/DebuggingSymbols.cpp \
			 kernel/multiboot.cpp \
			 arch/Paging.cpp \
			 arch/gdt.cpp \
			 arch/idt.cpp \
			 arch/pic.cpp  \
			 arch/pit.cpp  \
			 arch/hal.cpp  \
			 arch/interrupts.cpp \
			 kernel/Processes/Scheduler.cpp \
			 kernel/Processes/Thread.cpp \
			 kernel/DataStructures/OrderedArray.cpp \
			 arch/PageFaultHandler.cpp \
			 kernel/DataStructures/List.cpp\
			 kernel/Time/Timer.cpp\
			 kernel/Time/TimerManager.cpp \
			 kernel/Memory/Stack.cpp \
 			 kernel/Memory/PlacementAllocator.cpp \
             kernel/Memory/Virtual/VirtualMemoryManager.cpp \
             kernel/Memory/Virtual/VirtualMemorySpace.cpp \
             kernel/Memory/Virtual/VirtualMemoryRegion.cpp \
			 kernel/Memory/BitfieldPhysicalMemoryManager.cpp \
			 kernel/ElfInformation.cpp \
			 kernel/Processes/Scheduler.cpp \
			 kernel/Processes/Thread.cpp
#			 kernel/Memory/Slab/SlabManager.cpp

# List Assembler to be assembled with NASM here
ASOURCES = arch/start.S \
		   arch/interrupts_stubs.S \
		   arch/scheduling.S

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

# Main targets
all: createdirs $(TARGET)
	@:	#Shut up!

# sort for removing duplicates
$(sort $(addprefix $(OBJDIR)/, $(dir $(OBJECTS)))):
	$(call cmd_msg,MKDIR,$(@))
	$(Q)mkdir -p $@

createdirs: $(sort $(addprefix $(OBJDIR)/, $(dir $(OBJECTS))))

$(TARGET): $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(call cmd_msg,DISASM,$(@) -> disasm)
	$(Q)$(OBJDUMP) -d $(TARGET) > disasm

# Cleaning
clean:
	$(Q)rm -f $(TARGET)
#$(Q)rm -f $(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f disasm
	$(Q)rm -f kernel.map
	$(Q)rm -rf obj

distclean: clean

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

.PHONY: clean distclean
