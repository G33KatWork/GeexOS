SUBDIRS = testkernellib testkernel hello mbr stage1_fat16 stage2

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS)))
