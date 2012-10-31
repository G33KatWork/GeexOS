SUBDIRS = gnuefi libefi

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(abspath $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS))))
SELF_DIR := $(abspath $(SELF_DIR)/..)/
