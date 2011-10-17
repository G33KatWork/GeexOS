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
