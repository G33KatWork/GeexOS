SRC                 := ..
include $(SRC)/build/base.mak

# Main targets
all: libkcpp libarch kernel

libarch:
	$(call cmd_msg,SUBDIR,libarch)
	$(call call_submake,libarch,all)

libkcpp:
	$(call cmd_msg,SUBDIR,libkcpp)
	$(call call_submake,libkcpp,all)

kernel:
	$(call cmd_msg,SUBDIR,kernel)
	$(call call_submake,kernel,all)

# Cleaning
clean:
	$(call call_submake,libarch,clean)
	$(call call_submake,libkcpp,clean)
	$(call call_submake,kernel,clean)

distclean: clean
	$(call call_submake,libarch,distclean)
	$(call call_submake,libkcpp,distclean)
	$(call call_submake,kernel,toolchain-clean)

.PHONY: libkcpp libarch kernel clean distclean
