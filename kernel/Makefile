SRC                 := ..
include $(SRC)/build/base.mak

# Main targets
all: loaderstub kernel testmodule

loaderstub:
	$(call cmd_msg,SUBDIR,loaderstub)
	$(call call_submake,loaderstub,all)

libkcpp:
	$(call cmd_msg,SUBDIR,libkcpp)
	$(call call_submake,libkcpp,all)

libhalinterface:
	$(call cmd_msg,SUBDIR,libhalinterface)
	$(call call_submake,libhalinterface,all)

libhalx86:
	$(call cmd_msg,SUBDIR,libhalx86)
	$(call call_submake,libhalx86,all)

kernel: libkcpp libhalinterface libhalx86
	$(call cmd_msg,SUBDIR,kernel)
	$(call call_submake,kernel,all)

testmodule:
	$(call cmd_msg,SUBDIR,testmodule)
	$(call call_submake,testmodule,all)


# Floppy and execution
floppy.img: loaderstub kernel testmodule
	$(call cmd_msg,MKFLOPPY,floppy.img)
	$(Q)$(SUDO) -A $(MKDIR) tmp
	$(Q)$(SUDO) $(CP) ../resources/floppy_newKernel.$(FLOPPYTYPE).img ./floppy.img

ifeq ($(shell uname),Darwin)
	$(Q)$(SUDO) hdiutil attach floppy.img -mountpoint tmp -readwrite $(QOUTPUT)
else
	$(Q)$(SUDO) mount -o loop floppy.img tmp $(QOUTPUT)
endif

	$(Q)$(SUDO) $(CP) loaderstub/loaderstub.elf tmp/loader.elf
	$(Q)$(SUDO) $(CP) kernel/kernel.elf tmp/kern.elf
	$(Q)$(SUDO) utils/genmoduleblob.py testmodule/module.o > tmp/modules

ifeq ($(shell uname),Darwin)
	$(Q)$(SUDO) hdiutil detach tmp $(QOUTPUT)
else
	$(Q)$(SUDO) umount tmp $(QOUTPUT)
endif

	$(Q)$(SUDO) $(RM) -Rf tmp
	$(Q)$(SUDO) chmod 666 ./floppy.img

bochs: floppy.img
	$(call cmd_msg,BOCHS,floppy.img)
	$(Q)$(BOCHS) -f ../resources/bochsrc_newKernel.txt -q $(QOUTPUT)

qemu: floppy.img
	$(call cmd_msg,QEMU,floppy.img)
	$(Q)$(QEMU) -net none -fda floppy.img -serial file:serialOut $(QOUTPUT)

qemudebug: floppy.img
	$(call cmd_msg,QEMU,floppy.img)
	$(call cmd_msg,NOTE,Waiting for gdb attachment on port 1234...)
	$(Q)$(QEMU) -net none -fda floppy.img -serial file:serialOut -s -S $(QOUTPUT)


# Cleaning
clean:
	$(call call_submake,libkcpp,clean)
	$(call call_submake,libhalinterface,clean)
	$(call call_submake,libhalx86,clean)
	$(call call_submake,kernel,clean)
	$(call call_submake,loaderstub,clean)
	$(call call_submake,testmodule,clean)
	$(Q)$(RM) -f floppy.img

distclean: clean
	$(call call_submake,libkcpp,distclean)
	$(call call_submake,libhalinterface,distclean)
	$(call call_submake,libhalx86,distclean)
	$(call call_submake,kernel,distclean)
	$(call call_submake,loaderstub,distclean)
	$(call call_submake,testmodule,distclean)

.PHONY: testmodule loaderstub libhalinterface libhalx86 libkcpp kernel clean distclean floppy.img qemu qemudebug
