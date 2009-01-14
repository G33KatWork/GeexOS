SRC := .
include build/base.mak

# Target file name.
TARGET = kernel

# List C++ source files here.
SOURCES =  kernel/DebugDisplay.cpp \
       kernel/entry.cpp \
       kernel/main.cpp \
       kernel/exception.cpp \
       kernel/panic.cpp \
       kernel/paging.cpp \
       kernel/sortedArray.cpp \
       kernel/kheap.cpp \
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
ASOURCES = kernel/start.S \
	   hal/irqWrapper.S

# Addiotional include paths to consider
INCLUDES = inc

#Programs
#BOCHS = /opt/local/share/bochs/bochs.app/Contents/MacOS/bochs
#BOCHS = /Users/andy/Documents/devstuff/oskrempel/os/kernel/resources/bochs.app/Contents/MacOS/bochs
BOCHS = /usr/bin/bochs

# C compiler flags
CFLAGS += -g -fno-builtin -fno-exceptions -nostdlib -nostartfiles -nodefaultlibs
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
OBJECTS += $(ASOURCES:.S=.o)
OBJECTS += $(SOURCES:.cpp=.o)

# Main targets
all: toolchain $(TARGET).elf

$(TARGET).elf: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^
	$(OBJDUMP) --source $(TARGET).elf > source

# Toolchain
toolchain: $(CC)

$(CC):
	cd toolchain; make all

# Create bootfloppy
bootfloppy: all
	sudo mkdir tmp
	sudo cp resources/floppy.img .
	sudo mount -o loop floppy.img tmp
	sudo cp kernel.elf tmp/kernel
	sudo umount tmp
	sudo rm -Rf tmp
	
# Start bochs
bochs: bootfloppy
	$(BOCHS) -f resources/bochsrc.txt -q

# Start bochs with debugging support
debug:
	$(BOCHS) -f resources/bochsrcgdb.txt -q

gdb:
	gdb -q $(TARGET).elf -x resources/gdbinit

# Cleaning
clean:
	rm -f $(foreach ext,elf,$(TARGET).$(ext)) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	rm -f floppy.img
	rm -f bochsout.txt
	rm -f source
	rm -Rf tmp

distclean: clean
	cd toolchain; make distclean
	cd toolchain; make toolchain-clean

# Compile cpp files
%.o: $.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(NASM) $(ASFLAGS) -o $@ $<

