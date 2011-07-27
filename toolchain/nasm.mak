NASM_VERSION		:= 2.09
NASM_SOURCE	     	:= $(TOOLCHAIN_SRCDIR)/nasm-$(NASM_VERSION).tar.bz2
NASM_DOWNLOAD	    := http://www.nasm.us/pub/nasm/releasebuilds/$(NASM_VERSION)/nasm-$(NASM_VERSION).tar.bz2
NASM_PATCHES	    := 


# Download
$(NASM_SOURCE):
	$(call target_mkdir)
	$(call cmd_msg,WGET,$(subst $(SRC)/,,$(@)))
	$(Q)wget -c -O $(@).part $(NASM_DOWNLOAD)
	$(Q)mv $(@).part $(@)


# Extract
$(TOOLCHAIN_ROOTDIR)/.nasm-extract: $(NASM_SOURCE)
	$(Q)mkdir -p $(TOOLCHAIN_BUILDDIR)
	$(call cmd_msg,EXTRACT,$(subst $(SRC)/$(SRCSUBDIR)/,,$(NASM_SOURCE)))
	$(Q)tar -C $(TOOLCHAIN_BUILDDIR) -xjf $(NASM_SOURCE)
	$(call cmd_msg,PATCH,$(subst $(SRC)/$(SRCSUBDIR)/,,$(NASM_PATCHES)))
	$(Q)$(foreach patch,$(NASM_PATCHES), \
		cd $(TOOLCHAIN_BUILDDIR)/nasm-$(NASM_VERSION); \
		patch -Np1 -i $(patch) $(QOUTPUT); \
	)
	$(Q)touch $(@)


# Configure
$(TOOLCHAIN_ROOTDIR)/.nasm-configure: $(TOOLCHAIN_ROOTDIR)/.nasm-extract
	$(call cmd_msg,CONFIG,$(TOOLCHAIN_TARGET)/nasm-$(NASM_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/nasm-$(NASM_VERSION); \
		./configure \
		--prefix=$(TOOLCHAIN_ROOTDIR) \
		$(QOUTPUT)
	$(Q)touch $(@)


# Compile
$(TOOLCHAIN_ROOTDIR)/.nasm-compile: $(TOOLCHAIN_ROOTDIR)/.nasm-configure
	$(call cmd_msg,COMPILE,$(TOOLCHAIN_TARGET)/nasm-$(NASM_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/nasm-$(NASM_VERSION); $(MAKE) $(QOUTPUT)
	$(Q)touch $(@)


# Install
$(TOOLCHAIN_ROOTDIR)/.nasm-install: $(TOOLCHAIN_ROOTDIR)/.nasm-compile
	$(call cmd_msg,INSTALL,$(TOOLCHAIN_TARGET)/nasm-$(NASM_VERSION) ($(TOOLCHAIN_TARGET)))
	$(Q)cd $(TOOLCHAIN_BUILDDIR)/nasm-$(NASM_VERSION); $(MAKE) install $(QOUTPUT)
	$(Q)touch $(@)



# Download, build and install nasm to run on the host system.
NASM_TARGET := $(TOOLCHAIN_ROOTDIR)/.nasm-install
all-nasm: $(NASM_TARGET)
.PHONY: all-nasm

all: $(NASM_TARGET)
download: $(NASM_SOURCE)
