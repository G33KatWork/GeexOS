# Target file name.
TARGET = kernel

# List C++ source files here.
SRC =  kernel/DebugDisplay.cpp \
       kernel/entry.cpp \
       kernel/main.cpp \
       kernel/exception.cpp \
       kernel/panic.cpp \
       kernel/kmalloc.cpp \
       kernel/paging.cpp \
       lib/cstd.cpp \
       lib/string.cpp \
	   lib/stdio.cpp \
	   hal/cpu.cpp \
	   hal/gdt.cpp \
	   hal/hal.cpp \
	   hal/idt.cpp \
	   hal/pic.cpp \
	   hal/pit.cpp
	   

# List Assembler to be assembled with NASM here
ASRC = kernel/start.S \
	   hal/irqWrapper.S

# Addiotional include paths to consider
INCLUDES = inc

#Programs
CC = gcc
OBJDUMP = objdump
NASM = nasm
MKDIR = mkdir
RMDIR = rm -rf
RM = rm -f
CP = cp
MOUNT = mount
UMOUNT = umount
#BOCHS = /opt/local/share/bochs/bochs.app/Contents/MacOS/bochs
#BOCHS = /Users/andy/Documents/devstuff/oskrempel/os/kernel/resources/bochs.app/Contents/MacOS/bochs
BOCHS = /usr/bin/bochs

# Flags for automatic generation for dependencies
DEPFLAGS = -MD -MP -MF .dep/$(@F).d

# C compiler flags
CFLAGS += -g -fno-builtin -fno-exceptions -nostdlib -nostartfiles -nodefaultlibs $(DEPFLAGS)
CFLAGS += -Wall #-Wextra -Werror
CFLAGS += -DARCH_X86 -D_DEBUG #-fasm-blocks
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS = $(CFLAGS)

# NASM flags
ASFLAGS = -f elf

# Linker flags
LDFLAGS = -T linker.ld

# Determinte objects to be created
OBJECTS += $(ASRC:.S=.o)
OBJECTS += $(SRC:.cpp=.o)

# Main targets
all: $(TARGET).elf

$(TARGET).elf: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^
	$(OBJDUMP) --source $(TARGET).elf > source

# Create bootfloppy
bootfloppy: all
	sudo $(MKDIR) tmp
	sudo $(CP) resources/floppy.img .
	sudo $(MOUNT) -o loop floppy.img tmp
	sudo $(CP) kernel.elf tmp/kernel
	sudo $(UMOUNT) tmp
	sudo $(RMDIR) tmp
	
# Start bochs
bochs:
	$(BOCHS) -f resources/bochsrc.txt -q

# Start bochs with debugging support
debug:
	$(BOCHS) -f resources/bochsrcgdb.txt -q

gdb:
	gdb -q $(TARGET).elf -x resources/gdbinit

# Cleaning
clean:
	$(RM) $(foreach ext,elf,$(TARGET).$(ext)) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(RM) floppy.img
	$(RM) bochsout.txt
	$(RM) source
	$(RMDIR) tmp

distclean: clean
	$(RMDIR) .dep

# Compile cpp files
%.o: $.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(NASM) $(ASFLAGS) -o $@ $<

-include $(shell $(MKDIR) .dep 2>/dev/null) $(wildcard .dep/*)
