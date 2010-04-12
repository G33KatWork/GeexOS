SRC                 := .
include $(SRC)/build/base.mak

STARTTIME := $(shell date +%s)

# Main targets
all: kernel servers drivers applications
	$(call cmd_msg,NOTICE,Build completed in $$(($$(date +%s)-$(STARTTIME))) seconds)

kernel: toolchain
	$(call cmd_msg,SUBDIR,kernel)
	$(call call_submake,kernel,all)

servers: toolchain
	$(call cmd_msg,SUBDIR,servers)
	$(call call_submake,servers,all)

drivers: toolchain
	$(call cmd_msg,SUBDIR,drivers)
	$(call call_submake,drivers,all)

applications: toolchain
	$(call cmd_msg,SUBDIR,applications)
	$(call call_submake,applications,all)

# Toolchain
toolchain: $(CC)

$(CC):
	$(call cmd_msg,SUBDIR,toolchain)
	$(call call_submake,toolchain,all)

# Create bootfloppy
bootfloppy: all floppy.img

floppy.img: kernel initrd.img
	$(call cmd_msg,MKFLOPPY,floppy.img)
	$(Q)sudo mkdir tmp
	$(Q)sudo cp resources/floppy.$(FLOPPYTYPE).img ./floppy.img

ifeq ($(shell uname),Darwin)
	$(Q)sudo hdiutil attach floppy.img -mountpoint tmp -readwrite $(QOUTPUT)
else
	$(Q)sudo mount -o loop floppy.img tmp $(QOUTPUT)
endif

	$(Q)sudo cp kernel/kernel.elf tmp/kernel
	$(Q)sudo cp initrd.img tmp/initrd.img

ifeq ($(shell uname),Darwin)
	$(Q)sudo hdiutil detach tmp $(QOUTPUT)
else
	$(Q)sudo umount tmp $(QOUTPUT)
endif

	$(Q)sudo rm -Rf tmp

initrd.img: utils/geninitramfs
	$(call cmd_msg,GENINITRD,initrd.img)
	$(Q)utils/geninitramfs resources/test.txt test.txt resources/test1.txt test1.txt $(QOUTPUT)

utils/geninitramfs:
	$(call cmd_msg,SUBDIR,utils)
	$(call call_submake,utils,all)

# Start bochs
bochs: bootfloppy
	$(call cmd_msg,BOCHS,floppy.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

# Start qemu
qemu: bootfloppy
	$(call cmd_msg,QEMU,floppy.img)
	$(Q)$(QEMU) -net none -fda floppy.img -serial file:serialOut $(QOUTPUT)

qemudebug: bootfloppy
	$(call cmd_msg,QEMU,floppy.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -net none -fda floppy.img -serial file:serialOut -s -S $(QOUTPUT)

# Cleaning
clean:
	$(call call_submake,kernel,clean)
	$(call call_submake,toolchain,clean)
	$(call call_submake,utils,clean)
	$(call call_submake,applications,clean)
	$(call call_submake,drivers,clean)
	$(call call_submake,servers,clean)
	$(Q)rm -f floppy.img
	$(Q)rm -f initrd.img
	$(Q)rm -f bochsout.txt
	$(Q)rm -f source

distclean: clean
	$(call call_submake,kernel,distclean)
	$(call call_submake,toolchain,distclean)
	$(call call_submake,toolchain,toolchain-clean)

.PHONY: kernel servers drivers applications clean distclean qemu qemudebug bochs bochsdebug floppy.img initrd.img mkinitrd bootfloppy

