TOOLCHAIN_TARGET    := i586-elf

# UGLY!
SRC                 := ..
SRC                 := $(shell cd $(SRC); pwd)

TOOLCHAIN_ROOTDIR   := $(SRC)/toolchain/$(TOOLCHAIN_TARGET)
TOOLCHAIN_SRCDIR    := $(SRC)/toolchain/source
TOOLCHAIN_BUILDDIR  := $(TOOLCHAIN_ROOTDIR)/build-tmp
TOOLCHAIN_BINDIR    := $(TOOLCHAIN_ROOTDIR)/bin

include $(SRC)/toolchain/binutils.mak
#include $(SRC)/toolchain/gcc.mak

clean:
	if [ -n "$(TOOLCHAIN_BUILDDIR)" -a -d "$(TOOLCHAIN_BUILDDIR)" ]; then \
		rm -rf $(TOOLCHAIN_BUILDDIR); \
	fi

distclean: clean
	if [ -n "$(TOOLCHAIN_SRCDIR)" -a -d "$(TOOLCHAIN_SRCDIR)" ]; then \
		rm -rf $(TOOLCHAIN_SRCDIR); \
	fi

toolchain-clean: clean
	if [ -n "$(TOOLCHAIN_ROOTDIR)" -a -d "$(TOOLCHAIN_ROOTDIR)" ]; then \
		rm -rf $(TOOLCHAIN_ROOTDIR); \
	fi

.PHONY: download clean distclean toolchain-clean
.DEFAULT_GOAL = all
