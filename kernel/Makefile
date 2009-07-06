SRC                 := ..
include $(SRC)/build/base.mak

# Target file name.
TARGET = kernel.elf

# List C source files here.
CCSOURCES = 

# List C++ source files here
CXXSOURCES = lib/string.cpp \
             kernel/Memory/MemoryManager.cpp \
             kernel/main.cpp \
			 kernel/IO/Monitor.cpp \
			 kernel/global.cpp \
			 lib/cppruntime.cpp \
			 kernel/debug.cpp \
			 kernel/multiboot.cpp \
			 kernel/Memory/Paging.cpp \
			 arch/gdt.cpp \
			 arch/idt.cpp \
			 arch/pic.cpp  \
			 arch/pit.cpp  \
			 arch/hal.cpp  \
			 arch/interrupts.cpp \
			 kernel/Memory/Heap.cpp \
			 kernel/Processes/Scheduler.cpp \
			 kernel/Processes/Thread.cpp \
			 kernel/DataStructures/OrderedArray.cpp \
			 kernel/Memory/PageFaultHandler.cpp \
			 kernel/DataStructures/List.cpp\
			 kernel/Time/Timer.cpp\
			 kernel/Time/TimerManager.cpp \
			 kernel/Memory/Stack.cpp \
 			 kernel/Memory/BitfieldPhysicalMemoryManager.cpp \
 			 kernel/Memory/PlacementAllocator.cpp
#			 kernel/utils/demangle.cpp

# List D source files here
DSOURCES = 

# List Objective-C source files here
OBJCSOURCES = 

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
CFLAGS += -g -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=gnu99
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wstrict-prototypes #-Wconversion
CFLAGS += -DARCH_X86 -DDEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -g -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wredundant-decls -Winline -Wno-long-long \
			-Wconversion #-Werror
CXXFLAGS += -DARCH_X86 -DDEBUG
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# D compiler flags
DFLAGS += -nostdlib -nodefaultlibs -g #-mcmodel=kernel
DFLAGS += $(addprefix -I,$(INCLUDES))

# Objective-C compiler flags
OBJCFLAGS := $(CFLAGS) -fnext-runtime -Wno-import

# NASM flags
ASFLAGS = -f elf -g

# Linker flags
LDFLAGS = -T linker.ld -Map kernel.map

# Determinte objects to be created
OBJECTS += $(ASOURCES:%.S=%.o)
OBJECTS += $(GASOURCES:%.s=%.o)
OBJECTS += $(CXXSOURCES:%.cpp=%.o)
OBJECTS += $(CCSOURCES:%.c=%.o)
OBJECTS += $(DSOURCES:%.d=%.o)
OBJECTS += $(OBJCSOURCES:%.m=%.o)

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
	$(call cmd_msg,OBJDUMP,$(@) -> source)
	$(Q)$(OBJDUMP) --source $(TARGET) > source

# Cleaning
clean:
	$(Q)rm -f $(TARGET)
#$(Q)rm -f $(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f source
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

# Compile d files
$(OBJDIR)/%.o: $(SRCDIR)/%.d
	$(call cmd_msg,DC,$<)
	$(Q)$(DC) $(DFLAGS) -c $< -o $@

# Compile Objective-C files
$(OBJDIR)/%.o: $(SRCDIR)/%.m
	$(call cmd_msg,OBJCC,$<)
	$(Q)$(CC) $(OBJCFLAGS) -c $< -o $@

# Assembler S files
$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean distclean
