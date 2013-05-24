# Target file name.
TARGET = libkcpp

# List C source files here.
CCSOURCES = math_support.c

# List C++ source files here.
CXXSOURCES = cppruntime.cpp \
             string.cpp \
             typeinfo.cpp \
             stdlib.cpp

# List Files to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS = -ggdb

# C++ compiler flags
CXXFLAGS  = -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Additional include paths to consider
INCLUDES =	$(ROOT)/kernel/libkcpp/include

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG

include $(ROOT)/build/static-lib.mak
