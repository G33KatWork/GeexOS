SRC                 := .
include $(SRC)/build/base.mak

STARTTIME := $(shell date +%s)

# Main targets
all: bootloader kernel drivers servers applications
	$(call cmd_msg,NOTICE,Build completed in $$(($$(date +%s)-$(STARTTIME))) seconds)

kernel/kernel/kernel.elf: kernel

bootloader: toolchain
	$(call cmd_msg,SUBDIR,bootloader)
	$(call call_submake,bootloader,all)

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
toolchain: $(CC) $(LD) $(NASM) $(QEMU) $(GDB)

$(CC):
	$(call cmd_msg,SUBDIR,toolchain)
	$(call call_submake,toolchain,all)

# TODO: build script for automatic disk-image dependency rule creation
testhdd.img: all bootloader/mbr/mbr.bin bootloader/stage1_fat16/stage1.bin bootloader/stage2/stage2.bin
#create the target file by touching it. This has the nice effect that it belongs to the current user
#so, even if we need root rights to actually create the diskimage afterwards, it still belongs to the user and not root
	touch $@
ifeq ($(shell uname),Linux)
	$(SUDO) utils/buildhddimage.py utils/partlayout.json $@
else
	utils/buildhddimage.py utils/partlayout.json $@
endif

# Start bochs
bochs: testhdd.img
	$(call cmd_msg,BOCHS,testhdd.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

# Start qemu
qemu: testhdd.img
	$(call cmd_msg,QEMU,testhdd.img)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut $(QOUTPUT)

qemudebug: testhdd.img
	$(call cmd_msg,QEMU,testhdd.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut -s -S $(QOUTPUT)

ddd: all
	$(call cmd_msg,NOTE,Attaching ddd to port 1234)
	$(Q)ddd --debugger $(GDB) --command=resources/gdbinit

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

