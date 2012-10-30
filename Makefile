ROOT := $(PWD)
include $(ROOT)/build/base.mak

SUBDIRS = bootloader

STARTTIME := $(shell date +%s)
# Main targets
all: toolchain
	$(call cmd_msg,NOTICE,Build completed in $$(($$(date +%s)-$(STARTTIME))) seconds)
distclean: clean doxyclean

include $(addsuffix /GeexOS.mak,$(SUBDIRS))

.PHONY: all clean distclean bochs qemu qemudebug doxygen doxyclean

toolchain: $(CC) $(LD) $(NASM) $(QEMU) $(GDB)
$(CC) $(LD) $(NASM) $(QEMU) $(GDB):
	$(call cmd_msg,SUBDIR,toolchain)
	$(call call_submake,toolchain,all)

# TODO: build script for automatic disk-image dependency rule creation
testhdd.img: all $(BINARY-mbr) $(BINARY-stage1_fat16) $(BINARY-stage2)
#create the target file by touching it. This has the nice effect that it belongs to the current user
#so, even if we need root rights to actually create the diskimage afterwards, it still belongs to the user and not root
	touch $@
ifeq ($(shell uname),Linux)
	$(SUDO) utils/buildhddimage.py utils/partlayout.json $@
else
	utils/buildhddimage.py utils/partlayout.json $@
endif

bochs: testhdd.img
	$(call cmd_msg,BOCHS,testhdd.img)
	$(Q)$(BOCHS) -f resources/bochsrc.txt -q $(QOUTPUT)

qemu: testhdd.img
	$(call cmd_msg,QEMU,testhdd.img)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut $(QOUTPUT)

qemudebug: testhdd.img
	$(call cmd_msg,QEMU,testhdd.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -net none -hda testhdd.img -serial file:serialOut -s -S $(QOUTPUT)

doxygen: doxygen.conf
	$(call cmd_msg,DOXYGEN,$<)
	$(Q)cat $< | doxygen - $(QOUTPUT)

doxyclean:
	$(Q)$(RM) -rf doc/html

clean:
	$(Q)$(RM) -f testhdd.img
	$(call call_submake,toolchain,clean)

distclean:
	$(call call_submake,toolchain,distclean)
	$(call call_submake,toolchain,toolchain-clean)
