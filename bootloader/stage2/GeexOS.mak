SUBDIRS = stage2 stage15

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS)))
