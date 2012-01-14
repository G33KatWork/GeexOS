SRC                 := .
include $(SRC)/build/base.mak

STARTTIME := $(shell date +%s)

# Main targets
all: bootloader kernel drivers servers applications bootdisk
	$(call cmd_msg,NOTICE,Build completed in $$(($$(date +%s)-$(STARTTIME))) seconds)

kernel/kernel/kernel.elf: kernel

bootloader: toolchain
	$(call cmd_msg,SUBDIR,bootloader)
	$(call call_submake,bootloader,all)

kernel: toolchain
#	$(call cmd_msg,SUBDIR,kernel)
#	$(call call_submake,kernel,all)

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
toolchain: $(CC) $(LD) $(NASM) $(QEMU) $(BOCHS) $(GDB)

$(CC):
	$(call cmd_msg,SUBDIR,toolchain)
	$(call call_submake,toolchain,all)

# Disk for bootloader testing... incorporate into complete bootprocess later
bootdisk: bootloader
	utils/buildhddimage.py utils/partlayout.json testhdd.img

# Start bochs
bochs: all
	$(call cmd_msg,BOCHS,testhdd.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

# Start qemu
qemu: bootdisk
	$(call cmd_msg,QEMU,testhdd.img)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut $(QOUTPUT)

qemudebug: bootdisk
	$(call cmd_msg,QEMU,testhdd.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut -s -S $(QOUTPUT)

ddd: all
	$(call cmd_msg,NOTE,Attaching ddd to port 1234)
	$(Q)ddd --debugger toolchain/i686-elf/bin/i686-elf-gdb --command=resources/gdbinit

# Start VMware Fusion
vmware: all
	$(call cmd_msg,VMWARE,floppy.img)
	$(Q)/Applications/VMware\ Fusion.app/Contents/Library/vmrun -T fusion start resources/vmware.vmx $(QOUTPUT)

# generate doxygen documentation
doxygen: doxygen.conf
	$(call cmd_msg,DOXYGEN,$<)
	$(Q)cat $< | doxygen - $(QOUTPUT)

doxyclean:
	$(Q)$(RM) -rf doc/html

# Cleaning
clean:
	$(call call_submake,bootloader,clean)
	$(call call_submake,kernel,clean)
	$(call call_submake,toolchain,clean)
	$(call call_submake,applications,clean)
	$(call call_submake,drivers,clean)
	$(call call_submake,servers,clean)
	$(Q)$(RM) -f testhdd.img
	$(Q)$(RM) -f bochsout.txt
	$(Q)$(RM) -f source

distclean: clean doxyclean
	$(call call_submake,bootloader,distclean)
	$(call call_submake,kernel,distclean)
	$(call call_submake,toolchain,distclean)
	$(call call_submake,toolchain,toolchain-clean)

.PHONY: all bootloader kernel servers drivers applications clean distclean qemu qemudebug bochs bochsdebug doxygen

