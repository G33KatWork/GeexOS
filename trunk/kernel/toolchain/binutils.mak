BINUTILS_VERSION    := 2.19
BINUTILS_SOURCE     := $(TOOLCHAIN_SRCDIR)/binutils-$(BINUTILS_VERSION).tar.bz2
BINUTILS_DOWNLOAD   := http://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.bz2

# Download
$(BINUTILS_SOURCE):
	mkdir -p $(TOOLCHAIN_SRCDIR)
	wget -c -O $(@).part $(BINUTILS_DOWNLOAD)
	mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.binutils-extract: $(BINUTILS_SOURCE)
	mkdir -p $(TOOLCHAIN_BUILDDIR)
	tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(BINUTILS_SOURCE)
	touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.binutils-configure: $(TOOLCHAIN_ROOTDIR)/.binutils-extract
	if [ -d "$(TOOLCHAIN_BUILDDIR)/binutils-build" ]; then \
		rm -rf $(TOOLCHAIN_BUILDDIR)/binutils-build; \
	fi
	mkdir -p $(TOOLCHAIN_BUILDDIR)/binutils-build
	cd $(TOOLCHAIN_BUILDDIR)/binutils-build; \
		../binutils-$(BINUTILS_VERSION)/configure \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		--target=$(TOOLCHAIN_TARGET)
	touch  $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.binutils-compile: $(TOOLCHAIN_ROOTDIR)/.binutils-configure
	cd $(TOOLCHAIN_BUILDDIR)/binutils-build; $(MAKE) all
	touch $(@)

$(TOOLCHAIN_ROOTDIR)/.binutils-install: $(TOOLCHAIN_ROOTDIR)/.binutils-compile
	cd $(TOOLCHAIN_BUILDDIR)/binutils-build; $(MAKE) install
	touch $(@)



# Download, build and install binutils to run on the host system.
BINUTILS_TARGET := $(TOOLCHAIN_ROOTDIR)/.binutils-install
all-binutils: $(BINUTILS_TARGET)
.PHONY: all-binutils

all: $(BINUTILS_TARGET)
download: $(BINUTILS_SOURCE)
