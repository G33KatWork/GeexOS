SUBDIRS = hello mbr stage1_fat16 stage2 testkernellib testkernel

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(abspath $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS))))
SELF_DIR := $(abspath $(SELF_DIR)/..)/
