TOOLCHAIN_ROOTDIR   := $(SRC)/toolchain/$(TOOLCHAIN_TARGET)
TOOLCHAIN_SRCDIR    := $(SRC)/toolchain/source
TOOLCHAIN_BINDIR    := $(TOOLCHAIN_ROOTDIR)/bin
TOOLCHAIN_PATCHDIR	:= $(SRC)/toolchain/patches

AR      := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-ar
AS      := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-as
CC      := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-gcc
CXX     := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-g++
LD      := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-ld
NM      := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-nm
OBJCOPY := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-objcopy
OBJDUMP := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-objdump
RANLIB  := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-ranlib
READELF := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-readelf
STRIP   := $(TOOLCHAIN_BINDIR)/$(TOOLCHAIN_TARGET)-strip
NASM    := nasm
