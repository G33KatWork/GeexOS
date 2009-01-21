SRC := .
include build/base.mak

STARTTIME := $(shell date +%s)

# Target file name.
TARGET = kernel

# List C source files here.
CCSOURCES =	kernel/assert.c \
			kernel/DebugDisplay.c \
			kernel/entry.c \
			kernel/main.c \
			kernel/exception.c \
			kernel/panic.c \
			kernel/paging.c \
			kernel/sortedArray.c \
			kernel/kheap.c \
			kernel/fs.c \
			kernel/initrd.c \
			lib/string.c \
			lib/stdio.c \
			hal/cpu.c \
			hal/gdt.c \
			hal/hal.c \
			hal/idt.c \
			hal/pic.c \
			hal/pit.c

# List C++ source files here
CXXSOURCES =

# List Assembler to be assembled with NASM here
ASOURCES =	kernel/start.S \
			hal/irqWrapper.S

# Addiotional include paths to consider
INCLUDES =	inc

#Programs
#BOCHS = /opt/local/share/bochs/bochs.app/Contents/MacOS/bochs
#BOCHS = /Users/andy/Documents/devstuff/oskrempel/os/kernel/resources/bochs.app/Contents/MacOS/bochs
BOCHS = /usr/bin/bochs
QEMU = qemu

# C compiler flags
CFLAGS += -g -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=gnu99
CFLAGS += -Wall #-Wextra -Werror
CFLAGS += -DARCH_X86 -D_DEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -g -fno-builtin -fno-exceptions -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall #-Wextra -Werror
CXXFLAGS += -DARCH_X86 -D_DEBUG #-fasm-blocks
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# NASM flags
ASFLAGS = -f elf -g

# Linker flags
LDFLAGS = -T linker.ld

# Determinte objects to be created
OBJECTS += $(ASOURCES:.S=.o)
OBJECTS += $(CXXSOURCES:.cpp=.o)
OBJECTS += $(CCSOURCES:.c=.o)

# Main targets
all: toolchain $(TARGET).elf
	$(call cmd_msg,NOTICE,Build completed in $$(($$(date +%s)-$(STARTTIME))) seconds)

$(TARGET).elf: $(OBJECTS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(call cmd_msg,OBJDUMP,$(@) -> source)
	$(Q)$(OBJDUMP) --source $(TARGET).elf > source

# Toolchain
toolchain: $(CC)

$(CC):
	$(call cmd_msg,SUBDIR,toolchain)
	$(call call_submake,toolchain,all)

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
	$(call call_submake,utils,all)

# Start bochs
bochs: bootfloppy
	$(call cmd_msg,BOCHS,floppy.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

# Start bochs with debugging support
bochsdebug: bootfloppy
	$(call cmd_msg,BOCHSDBG,floppy.img)
	$(Q)$(BOCHS) -f resources/bochsrcgdb.txt -q $(QOUTPUT) &
	$(Q)sleep 2
	$(call cmd_msg,GDB,Attaching debugger)
	#$(Q)gdb -q $(TARGET).elf -x resources/gdbinit

# Start qemu
qemu: bootfloppy
	$(call cmd_msg,QEMU,floppy.img)
	$(Q)$(QEMU) -fda floppy.img $(QOUTPUT)

qemudebug: bootfloppy
	$(call cmd_msg,QEMU,floppy.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -fda floppy.img -s -S $(QOUTPUT)
	#$(Q)gdb -q $(TARGET).elf -x resources/gdbinit

# Cleaning
clean:
	$(Q)rm -f $(foreach ext,elf,$(TARGET).$(ext)) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f floppy.img
	$(Q)rm -f initrd.img
	$(Q)rm -f bochsout.txt
	$(Q)rm -f source
	$(Q)rm -Rf tmp
	$(call call_submake,toolchain,clean)
	$(call call_submake,utils,clean)

distclean: clean
	$(call call_submake,toolchain,distclean)
	$(call call_submake,toolchain,toolchain-clean)

# Compile cpp files
%.o: %.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile c files
%.o: %.c
	$(call cmd_msg,CC,$<)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean distclean qemu qemudebug bochs bochsdebug floppy.img initrd.img mkinitrd bootfloppy

