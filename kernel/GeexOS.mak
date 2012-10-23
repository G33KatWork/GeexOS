SUBDIRS = libhalinterface libhalx86 libkcpp kernel

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(addprefix $(SELF_DIR),$(addsuffix /GeexOS.mak,$(SUBDIRS)))
