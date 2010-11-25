SRC                 := ../..
include $(SRC)/build/base.mak

# Target file name.
TARGET = kernel.elf

# List C source files here.
CCSOURCES = 

# List C++ source files here
CXXSOURCES = main.cpp \
			 global.cpp \
			 Processes/Scheduler.cpp \
			 Processes/Thread.cpp \
			 DataStructures/OrderedArray.cpp \
			 DataStructures/List.cpp\
			 Time/Timer.cpp\
			 Time/TimerManager.cpp \
 			 Memory/PlacementAllocator.cpp \
             Memory/Virtual/VirtualMemoryManager.cpp \
             Memory/Virtual/VirtualMemorySpace.cpp \
             Memory/Virtual/VirtualMemoryRegion.cpp \
			 Memory/BitfieldPhysicalMemoryManager.cpp \
			 Memory/IO/IOMemoryManager.cpp \
			 utils/ElfInformation.cpp \
			 Processes/Scheduler.cpp \
			 Processes/Thread.cpp \
			 Processes/KernelThread.cpp \
			 Memory/Virtual/Regions/KernelStackMemoryRegion.cpp \
			 Memory/Virtual/Regions/KernelThreadStackRegion.cpp \
			 Memory/Slab/SlabAllocator.cpp \
			 Memory/Slab/LargeCache.cpp \
			 Memory/Slab/SmallCache.cpp \
			 Memory/Slab/SlabCache.cpp \
             DataStructures/Bitfield.cpp \
 			 Memory/Virtual/Regions/BuddyAllocatedMemoryRegion.cpp
#			 Memory/Slab/SlabManager.cpp

# List Assembler to be assembled with NASM here
ASOURCES = 

# Additional include paths to consider
INCLUDES =	include ../libarchx86/include ../libkcpp/include ../libhalinterface/include

# Additional static libs to link agains
STATICLIBS = ../libarchx86/libarchx86.a ../libkcpp/libkcpp.a ../libhalinterface/libhalinterface.a

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
			-Wwrite-strings -Wredundant-decls -Winline -Wno-long-long
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

$(TARGET): $(addprefix $(OBJDIR)/, $(OBJECTS)) $(STATICLIBS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(STATICLIBS)
	$(call cmd_msg,DISASM,$(@) -> disasm)
	$(Q)$(OBJDUMP) -d $(TARGET) > disasm
	$(call cmd_msg,MKSYMS,$(@) -> bochs.sym)
	$(Q)$(NM) $@ | awk '{print $1 " " $3}' > bochs.sym

# Cleaning
clean:
	$(Q)$(RM) -f $(TARGET)
#$(Q)$(RM) -f $(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)$(RM) -f disasm
	$(Q)$(RM) -f kernel.map
	$(Q)$(RM) -rf obj
	$(Q)$(RM) -rf bochs.sym

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
