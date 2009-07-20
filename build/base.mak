# Prevent multiple includes.
ifneq ($(BASE_INCLUDED),1)
BASE_INCLUDED := 1

# Fancy colorful output
BUILD_COLOUR ?= 1
export BUILD_COLOUR V

# Be verbose while building only if V is set to 1 in the environment.
ifneq ($(V),1)
  Q := @
  QOUTPUT := >> /dev/null 2>&1
  MAKEFLAGS += --no-print-directory
endif

#####################
# Utility functions #
#####################

# Call Make in a subdirectory with the necessary variables passed to it.
# @param 1	Directory to call Make in.
# @param 2	Extra arguments to pass to Make (targets, etc).
define call_submake
$(Q)+$(MAKE) -C $(1) $(2)
endef

# Print a message for a command, e.g. "-> CC      foo.c"
# @param 1	Command name.
# @param 2	Extra message (e.g. file name).
ifneq ($(V),1)
  ifneq ($(BUILD_COLOUR),1)
    define cmd_msg
    @printf "[%-8s] %-$$(($(MAKELEVEL)*2))s$(2)\n" $(1) ""
    endef
  else
    define cmd_msg
    @printf "\033[1;37m[\033[1;34m%-8s\033[1;37m] \033[0;1m%-$$(($(MAKELEVEL)*2))s$(2)\033[0m\n" $(1) ""
    endef
  endif
endif

# Create the directory that the target will go in if non-existant.
define target_mkdir
@mkdir -p $(dir $(@))
endef

############################
# Core build configuration #
############################

# Define toolchain target
TOOLCHAIN_TARGET    := i686-elf
#TOOLCHAIN_TARGET    := avr32

# Define root directory
SRC                 := $(shell cd $(SRC); pwd)

# Grab the toolchain information.
include $(SRC)/build/toolchain.mak

# FS-Type for floppy image
FLOPPYTYPE			:= fat
#FLOPPYTYPE			:= ext2

############################
# Host build configuration #
############################

HOSTCC      := gcc
HOSTCXX     := g++
HOSTCFLAGS  := 

endif
