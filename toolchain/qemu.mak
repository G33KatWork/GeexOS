QEMU_VERSION		:= 1.2.0
QEMU_SOURCE	     	:= $(TOOLCHAIN_SRCDIR)/qemu-$(QEMU_VERSION).tar.bz2
QEMU_DOWNLOAD	    := http://wiki.qemu.org/download/qemu-$(QEMU_VERSION).tar.bz2
QEMU_PATCHES	    := 

# Download
$(QEMU_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(ROOT)/,,$(@)))
	$(Q)wget -c -O $(@).part $(QEMU_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.qemu-extract: $(QEMU_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(ROOT)/$(SRCSUBDIR)/,,$(QEMU_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(QEMU_SOURCE)
	$(call cmd_msg,PATCH,$(subst $(ROOT)/$(SRCSUBDIR)/,,$(QEMU_PATCHES)))
	$(Q)$(foreach patch,$(QEMU_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT); \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.qemu-configure: $(TOOLCHAIN_ROOTDIR)/.qemu-extract
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/qemu-$(QEMU_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); \
		$(QEMU_CONFENV) ./configure \
		--enable-sdl \
		--disable-kvm \
		--enable-system \
		--disable-linux-user \
		--disable-bsd-user \
		--target-list="i386-softmmu x86_64-softmmu" \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		$(QOUTPUT)
	$(Q)touch $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.qemu-compile: $(TOOLCHAIN_ROOTDIR)/.qemu-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/qemu-$(QEMU_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); $(MAKE) $(QOUTPUT)
	$(Q)touch $(@)


# Install
$(TOOLCHAIN_ROOTDIR)/.qemu-install: $(TOOLCHAIN_ROOTDIR)/.qemu-compile
	$(call cmd_msg,INSTALL,$(TOOLCHAIN_TARGET)/qemu-$(QEMU_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); $(MAKE) install $(QOUTPUT)
	$(Q)touch $(@)


# Download, build and install qemu to run on the host system.
QEMU_TARGET := $(TOOLCHAIN_ROOTDIR)/.qemu-install
all-qemu: $(QEMU_TARGET)
.PHONY: all-qemu

all: $(QEMU_TARGET)
download: $(QEMU_SOURCE)

