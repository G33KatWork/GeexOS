BOCHS_VERSION		:= 2.6.1
BOCHS_SOURCE	    := $(TOOLCHAIN_SRCDIR)/bochs-$(BOCHS_VERSION).tar.gz
BOCHS_DOWNLOAD	    := http://downloads.sourceforge.net/project/bochs/bochs/$(BOCHS_VERSION)/bochs-$(BOCHS_VERSION).tar.gz
BOCHS_PATCHES	    := 

# Download
$(BOCHS_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(ROOT)/,,$(@)))
	$(Q)wget -c -O $(@).part $(BOCHS_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.bochs-extract: $(BOCHS_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(ROOT)/$(SRCSUBDIR)/,,$(BOCHS_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xzf $(BOCHS_SOURCE)
	$(call cmd_msg,PATCH,$(subst $(ROOT)/$(SRCSUBDIR)/,,$(BOCHS_PATCHES)))
	$(Q)$(foreach patch,$(BOCHS_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/bochs-$(BOCHS_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT); \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.bochs-configure: $(TOOLCHAIN_ROOTDIR)/.bochs-extract
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/bochs-$(BOCHS_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/bochs-$(BOCHS_VERSION); \
		LDFLAGS=-lpthread ./configure \
		--with-sdl \
	  --with-x11 \
		--enable-debugger \
		--enable-disasm \
		--enable-debugger-gui \
		--enable-smp \
		--enable-x86-64 \
		--enable-pci \
		--enable-fpu \
		--enable-usb \
		--enable-a20-pin \
		--enable-show-ips \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		$(QOUTPUT)
	$(Q)touch $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.bochs-compile: $(TOOLCHAIN_ROOTDIR)/.bochs-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/bochs-$(BOCHS_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/bochs-$(BOCHS_VERSION); $(MAKE) $(QOUTPUT)
	$(Q)touch $(@)


# Install
$(TOOLCHAIN_ROOTDIR)/.bochs-install: $(TOOLCHAIN_ROOTDIR)/.bochs-compile
	$(call cmd_msg,INSTALL,$(TOOLCHAIN_TARGET)/bochs-$(BOCHS_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/bochs-$(BOCHS_VERSION); $(MAKE) install $(QOUTPUT)
	$(Q)touch $(@)


# Download, build and install bochs to run on the host system.
BOCHS_TARGET := $(TOOLCHAIN_ROOTDIR)/.bochs-install
all-bochs: $(BOCHS_TARGET)
.PHONY: all-bochs

all: $(BOCHS_TARGET)
download: $(BOCHS_SOURCE)

