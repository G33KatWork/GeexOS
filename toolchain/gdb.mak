GDB_VERSION    := 7.0
GDB_SOURCE     := $(TOOLCHAIN_SRCDIR)/gdb-$(GDB_VERSION).tar.bz2
GDB_DOWNLOAD   := http://ftp.gnu.org/gnu/gdb/gdb-$(GDB_VERSION).tar.bz2
GDB_PATCHES    := 

ifeq ($(TOOLCHAIN_TARGET),avr32)
GDB_PATCHES += $(TOOLCHAIN_PATCHDIR)/gdb-$(GDB_VERSION).atmel.1.0.3.patch
endif

# Download
$(GDB_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(SRC)/,,$(@)))
	$(Q)wget -c -O $(@).part $(GDB_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.gdb-extract: $(GDB_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(GDB_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(GDB_SOURCE)
	$(call cmd_msg,PATCH,$(subst $(SRC)/$(SRCSUBDIR)/,,$(GDB_PATCHES)))
	$(Q)$(foreach patch,$(GDB_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/gdb-$(GDB_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT); \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.gdb-configure: $(TOOLCHAIN_ROOTDIR)/.gdb-extract
	$(Q)if [ -d "$(TOOLCHAIN_BUILDDIR)/gdb-build" ]; then \
		rm -rf $(TOOLCHAIN_BUILDDIR)/gdb-build; \
	fi
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)/gdb-build
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/gdb-$(GDB_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gdb-build; \
		../gdb-$(GDB_VERSION)/configure \
		--disable-werror \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		--target=$(TOOLCHAIN_TARGET) \
		$(QOUTPUT)
	$(Q)touch $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.gdb-compile: $(TOOLCHAIN_ROOTDIR)/.gdb-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/gdb-$(GDB_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gdb-build; $(MAKE) all $(QOUTPUT)
	$(Q)touch $(@)


# Install
$(TOOLCHAIN_ROOTDIR)/.gdb-install: $(TOOLCHAIN_ROOTDIR)/.gdb-compile
	$(call cmd_msg,INSTALL,$(TOOLCHAIN_TARGET)/gdb-$(GDB_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/gdb-build; $(MAKE) install $(QOUTPUT)
	$(Q)touch $(@)



# Download, build and install gdb to run on the host system.
GDB_TARGET := $(TOOLCHAIN_ROOTDIR)/.gdb-install
all-gdb: $(GDB_TARGET)
.PHONY: all-gdb

all: $(GDB_TARGET)
download: $(GDB_SOURCE)
