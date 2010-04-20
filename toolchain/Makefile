SRC                 := ..
include $(SRC)/build/base.mak

TOOLCHAIN_BUILDDIR  := $(TOOLCHAIN_ROOTDIR)/build-tmp

include $(SRC)/toolchain/binutils.mak
include $(SRC)/toolchain/nasm.mak
include $(SRC)/toolchain/gcc.mak
include $(SRC)/toolchain/gdb.mak
include $(SRC)/toolchain/qemu.mak
include $(SRC)/toolchain/bochs.mak

clean:
	$(Q)if [ -n "$(TOOLCHAIN_BUILDDIR)" -a -d "$(TOOLCHAIN_BUILDDIR)" ]; then \
		rm -rf $(TOOLCHAIN_BUILDDIR); \
	fi

distclean: clean
	$(Q)if [ -n "$(TOOLCHAIN_SRCDIR)" -a -d "$(TOOLCHAIN_SRCDIR)" ]; then \
		rm -rf $(TOOLCHAIN_SRCDIR); \
	fi

toolchain-clean: clean
	$(Q)if [ -n "$(TOOLCHAIN_ROOTDIR)" -a -d "$(TOOLCHAIN_ROOTDIR)" ]; then \
		rm -rf $(TOOLCHAIN_ROOTDIR); \
	fi

.PHONY: download clean distclean toolchain-clean
.DEFAULT_GOAL = all
