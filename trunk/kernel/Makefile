SRC := .
include build/base.mak

# Target file name.
TARGET = kernel

# List C++ source files here.
SOURCES =	kernel/DebugDisplay.cpp \
			kernel/entry.cpp \
			kernel/main.cpp \
			kernel/exception.cpp \
			kernel/panic.cpp \
			kernel/paging.cpp \
			kernel/sortedArray.cpp \
			kernel/kheap.cpp \
			kernel/fs.cpp \
			kernel/initrd.cpp \
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
ASOURCES =	kernel/start.S \
			hal/irqWrapper.S

# Addiotional include paths to consider
INCLUDES =	inc

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
	$(call cmd_msg,LINK,$(@))
	$(Q)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^
	$(call cmd_msg,OBJDUMP,$(@) -> source)
	$(Q)$(OBJDUMP) --source $(TARGET).elf > source

# Toolchain
toolchain: $(CC)

$(CC):
	$(call cmd_msg,SUBDIR,toolchain)
	$(Q)cd toolchain; make all

# Create bootfloppy
bootfloppy: all mkinitrd floppy.img

floppy.img:
	$(call cmd_msg,MKFLOPPY,floppy.img)
	$(Q)sudo mkdir tmp
	$(Q)sudo cp resources/floppy.img .
	$(Q)sudo mount -o loop floppy.img tmp
	$(Q)sudo cp kernel.elf tmp/kernel
	$(Q)sudo cp initrd.img tmp/initrd.img
	$(Q)sudo umount tmp
	$(Q)sudo rm -Rf tmp

mkinitrd: utils/geninitramfs initrd.img

initrd.img:
	$(call cmd_msg,GENINITRD,initrd.img)
	$(Q)utils/geninitramfs resources/test.txt test.txt resources/test1.txt test1.txt $(QOUTPUT)

utils/geninitramfs:
	$(call cmd_msg,SUBDIR,utils)
	$(Q)cd utils; make all

# Start bochs
bochs: bootfloppy
	$(call cmd_msg,BOCHS,floppy.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

# Start bochs with debugging support
debug: bootfloppy
	$(call cmd_msg,BOCHSDBG,floppy.img)
	$(Q)$(BOCHS) -f resources/bochsrcgdb.txt -q $(QOUTPUT) &
	$(Q)sleep 2
	$(call cmd_msg,GDB,Attaching debugger)
	$(Q)gdb -q $(TARGET).elf -x resources/gdbinit

# Cleaning
clean:
	$(Q)rm -f $(foreach ext,elf,$(TARGET).$(ext)) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f floppy.img
	$(Q)rm -f initrd.img
	$(Q)rm -f bochsout.txt
	$(Q)rm -f source
	$(Q)rm -Rf tmp
	$(Q)cd toolchain; make clean
	$(Q)cd utils; make clean

distclean: clean
	$(Q)cd toolchain; make distclean
	$(Q)cd toolchain; make toolchain-clean

# Compile cpp files
%.o: %.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

