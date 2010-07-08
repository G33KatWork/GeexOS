SRC                 := ..
include $(SRC)/build/base.mak

# Main targets
all: libkcpp libarchx86 libhalinterface kernel

libarchx86:
	$(call cmd_msg,SUBDIR,libarchx86)
	$(call call_submake,libarchx86,all)

libhalinterface:
	$(call cmd_msg,SUBDIR,libhalinterface)
	$(call call_submake,libhalinterface,all)

libkcpp:
	$(call cmd_msg,SUBDIR,libkcpp)
	$(call call_submake,libkcpp,all)

kernel:
	$(call cmd_msg,SUBDIR,kernel)
	$(call call_submake,kernel,all)

# Cleaning
clean:
	$(call call_submake,libarchx86,clean)
	$(call call_submake,libhalinterface,clean)
	$(call call_submake,libkcpp,clean)
	$(call call_submake,kernel,clean)

distclean: clean
	$(call call_submake,libarchx86,distclean)
	$(call call_submake,libhalinterface,distclean)
	$(call call_submake,libkcpp,distclean)
	$(call call_submake,kernel,toolchain-clean)

.PHONY: libkcpp libarchx86 libhalinterface kernel clean distclean
