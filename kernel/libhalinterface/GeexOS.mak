# Target file name.
TARGET = libhalinterface

# List C source files here.
CCSOURCES = 

# List C++ source files here.
CXXSOURCES = BaseOutputDevice.cpp

# List Files to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS =

# C++ compiler flags
CXXFLAGS  = -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Additional include paths to consider
INCLUDES =	$(ROOT)/kernel/libhalinterface/include \
            $(ROOT)/kernel/libkcpp/include

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG

include $(ROOT)/build/static-lib.mak
