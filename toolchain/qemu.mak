QEMU_VERSION		:= 0.14.1
QEMU_SOURCE	     	:= $(TOOLCHAIN_SRCDIR)/qemu-$(QEMU_VERSION).tar.gz
QEMU_DOWNLOAD	    := http://download.savannah.gnu.org/releases/qemu/qemu-$(QEMU_VERSION).tar.gz
QEMU_PATCHES	    := 

# Hack to build on OS X.
ifeq ($(shell uname),Darwin)
# fix compilation issue with llvm/clang (segfault at runtime)
QEMU_CONFENV = CC=/usr/bin/gcc-4.2 CPP=/usr/bin/cpp-4.2 CXX=/usr/bin/g++-4.2 LD=/usr/bin/gcc-4.2
endif

# Download
$(QEMU_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(SRC)/,,$(@)))
	$(Q)wget -c -O $(@).part $(QEMU_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.qemu-extract: $(QEMU_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(QEMU_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xzf $(QEMU_SOURCE)
	$(call cmd_msg,PATCH,$(subst $(SRC)/$(SRCSUBDIR)/,,$(QEMU_PATCHES)))
	$(Q)$(foreach patch,$(QEMU_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT); \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.qemu-configure: $(TOOLCHAIN_ROOTDIR)/.qemu-extract
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/qemu-$(QEMU_VERSION) ($(TOOLCHAIN_TARGET)))
ifeq ($(shell uname),Darwin)
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); \
		$(QEMU_CONFENV) ./configure \
		--enable-cocoa \
		--disable-sdl \
		--disable-xen \
		--disable-kvm \
		--enable-system \
		--disable-linux-user \
		--disable-darwin-user \
		--disable-bsd-user \
		--target-list="i386-softmmu x86_64-softmmu" \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		$(QOUTPUT)
else
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/qemu-$(QEMU_VERSION); \
		$(QEMU_CONFENV) ./configure \
		--disable-cocoa \
		--enable-sdl \
		--disable-xen \
		--disable-kqemu \
		--disable-kvm \
		--enable-system \
		--disable-linux-user \
		--disable-darwin-user \
		--disable-bsd-user \
		--target-list="i386-softmmu x86_64-softmmu" \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		$(QOUTPUT)
endif
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

