GCC_VERSION    := 4.1.2
GDC_VERSION    := 0.24
GCC_SOURCE     := $(TOOLCHAIN_SRCDIR)/gcc-$(GCC_VERSION).tar.bz2
GDC_SOURCE     := $(TOOLCHAIN_SRCDIR)/gdc-$(GDC_VERSION).tar.bz2
GCC_DOWNLOAD   := http://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2
GDC_DOWNLOAD   := http://freefr.dl.sourceforge.net/sourceforge/dgcc/gdc-$(GDC_VERSION)-src.tar.bz2
GDC_PATCHES    := $(TOOLCHAIN_PATCHDIR)/gdc-0.24-cygwin_d_os_versym-define.patch

PATH += :$(TOOLCHAIN_ROOTDIR)/bin

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
	
$(GDC_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(SRC)/,,$(@)))
	$(Q)wget -c -O $(@).part $(GDC_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.gcc-extract: $(GCC_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(GCC_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(GCC_SOURCE)
	$(Q)touch $(@)
	
$(TOOLCHAIN_ROOTDIR)/.gdc-extract: $(GDC_SOURCE) $(TOOLCHAIN_ROOTDIR)/.gcc-extract
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(GDC_SOURCE)))
	$(Q)mkdir $(TOOLCHAIN_BUILDDIR)/gcc-$(GCC_VERSION)/gcc/d
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR)/gcc-$(GCC_VERSION)/gcc -xjf $(GDC_SOURCE)
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gcc-$(GCC_VERSION); ./gcc/d/setup-gcc.sh $(QOUTPUT)
	$(Q)$(foreach patch,$(GDC_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/gcc-$(GCC_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT) \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.gcc-configure: $(TOOLCHAIN_ROOTDIR)/.gcc-extract $(TOOLCHAIN_ROOTDIR)/.gdc-extract
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
			--enable-languages=c,d,c++ --disable-nls \
			$(QOUTPUT) \
			$(GCC_CONFOPTS)
	$(Q)touch  $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.gcc-compile: $(TOOLCHAIN_ROOTDIR)/.gcc-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/gcc-$(GCC_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gcc-build; \
		$(MAKE) all-gcc $(QOUTPUT)
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
