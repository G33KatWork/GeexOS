GCC_VERSION    := 4.3.2
GCC_SOURCE     := $(TOOLCHAIN_SRCDIR)/gcc-$(GCC_VERSION).tar.bz2
GCC_DOWNLOAD   := http://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2

# Hack to build on OS X.
ifeq ($(shell uname),Darwin)
  GCC_CONFOPTS := --with-gmp=/opt/local --with-mpfr=/opt/local
endif

# Download
$(GCC_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(SRC)/,,$(@)))
	$(Q)wget -c -O $(@).part $(GCC_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.gcc-extract: $(GCC_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(GCC_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(GCC_SOURCE)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.gcc-configure: $(TOOLCHAIN_ROOTDIR)/.gcc-extract
	$(Q)if [ -d "$(TOOLCHAIN_BUILDDIR)/gcc-build" ]; then \
		rm -rf $(TOOLCHAIN_BUILDDIR)/gcc-build; \
	fi
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)/gcc-build
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/gcc-$(GCC_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gcc-build; \
		../gcc-$(GCC_VERSION)/configure \
			--prefix=$(TOOLCHAIN_ROOTDIR) \
			--target=$(TOOLCHAIN_TARGET) \
			--without-headers \
			--enable-languages=c,c++ --disable-nls \
			$(QOUTPUT)
	$(Q)touch  $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.gcc-compile: $(TOOLCHAIN_ROOTDIR)/.gcc-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/gcc-$(GCC_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gcc-build; $(MAKE) all-gcc $(QOUTPUT)
	$(Q)touch $(@)


# Install
$(TOOLCHAIN_ROOTDIR)/.gcc-install: $(TOOLCHAIN_ROOTDIR)/.gcc-compile
	$(call cmd_msg,INSTALL,$(TOOLCHAIN_TARGET)/gcc-$(GCC_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gcc-build; $(MAKE) install-gcc $(QOUTPUT)
	$(Q)touch $(@)



# Download, build and install binutils to run on the host system.
GCC_TARGET := $(TOOLCHAIN_ROOTDIR)/.gcc-install
all-gcc: $(BINUTILS_TARGET)
.PHONY: all-gcc

all: $(GCC_TARGET)
download: $(GCC_SOURCE)