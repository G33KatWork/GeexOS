SUBDIRS-generic := testkernellib testkernel
SUBDIRS-ia32 	:= hello mbr stage1_fat16 stage2 gnuefi efitest

SUBDIRS := $(SUBDIRS-generic) $(SUBDIRS-$(ARCH))
SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(abspath $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS))))
SELF_DIR := $(abspath $(SELF_DIR)/..)/
