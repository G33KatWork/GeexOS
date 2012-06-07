# Prevent multiple includes.
ifneq ($(BASE_INCLUDED),1)
BASE_INCLUDED := 1

# Define root directory
SRC := $(shell cd $(SRC); pwd)

# Fancy colorful output
BUILD_COLOUR ?= 1
export BUILD_COLOUR V

# Be verbose while building only if V is set to 1 in the environment.
ifneq ($(V),1)
  Q := @
  QOUTPUT := >> /dev/null 2>&1
  MAKEFLAGS += --no-print-directory
endif

############################
# General utilities	       #
############################
ifeq ($(shell uname),Darwin)
	export SUDO_ASKPASS:= $(SRC)/build/macos-askpass
SUDO		:= sudo -A
else
SUDO		:= sudo
endif

RM			:= rm
MKDIR		:= mkdir
CP			:= cp

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
@$(MKDIR) -p $(dir $(@))
endef

############################
# Core build configuration #
############################

# Define toolchain target
TOOLCHAIN_TARGET    := i686-pc-mingw32
#TOOLCHAIN_TARGET    := x86_64-w64-mingw32
#TOOLCHAIN_TARGET    := i686-elf

# Grab the toolchain information.
include $(SRC)/build/toolchain.mak

############################
# Host build configuration #
############################

HOSTCC      := gcc
HOSTCXX     := g++
HOSTCFLAGS  := -std=gnu99

endif
