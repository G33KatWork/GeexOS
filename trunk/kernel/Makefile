SRC                 := ..
include $(SRC)/build/base.mak

# Target file name.
TARGET = kernel.elf

# List C source files here.
CCSOURCES = 

# List C++ source files here
CXXSOURCES = src/lib/string.cpp \
             src/kernel/Memory/MemoryManager.cpp \
             src/kernel/main.cpp \
			 src/kernel/IO/Monitor.cpp \
			 src/kernel/global.cpp \
			 src/lib/cppruntime.cpp \
			 src/kernel/debug.cpp \
			 src/kernel/multiboot.cpp \
			 src/kernel/Memory/Paging.cpp \
			 src/arch/gdt.cpp \
			 src/arch/idt.cpp \
			 src/arch/pic.cpp  \
			 src/arch/pit.cpp  \
			 src/arch/hal.cpp  \
			 src/arch/interrupts.cpp \
			 src/kernel/Memory/Heap.cpp \
			 src/kernel/Processes/Scheduler.cpp \
			 src/kernel/Processes/Process.cpp \
			 src/kernel/DataStructures/OrderedArray.cpp \
			 src/kernel/Memory/PageFaultHandler.cpp \
			 src/kernel/DataStructures/List.cpp\
			 src/kernel/Time/Timer.cpp\
			 src/kernel/Time/TimerManager.cpp

# List D source files here
DSOURCES = 

# List Objective-C source files here
OBJCSOURCES = 

# List Assembler to be assembled with NASM here
ASOURCES = src/arch/start.S \
		   src/arch/interrupts_stubs.S \
		   src/arch/scheduling.S

# Addiotional include paths to consider
INCLUDES =	include

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
OBJECTS += $(ASOURCES:.S=.o)
OBJECTS += $(CXXSOURCES:.cpp=.o)
OBJECTS += $(CCSOURCES:.c=.o)
OBJECTS += $(DSOURCES:.d=.o)
OBJECTS += $(OBJCSOURCES:.m=.o)

# Main targets
all: $(TARGET)
	@:	#Shut up!

$(TARGET): $(OBJECTS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(call cmd_msg,OBJDUMP,$(@) -> source)
	$(Q)$(OBJDUMP) --source $(TARGET) > source

# Cleaning
clean:
	$(Q)rm -f $(TARGET) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f source
	$(Q)rm -f kernel.map

distclean: clean

# Compile cpp files
%.o: %.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile c files
%.o: %.c
	$(call cmd_msg,CC,$<)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Compile d files
%.o: %.d
	$(call cmd_msg,DC,$<)
	$(Q)$(DC) $(DFLAGS) -c $< -o $@

# Compile Objective-C files
%.o: %.m
	$(call cmd_msg,OBJCC,$<)
	$(Q)$(CC) $(OBJCFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean distclean
