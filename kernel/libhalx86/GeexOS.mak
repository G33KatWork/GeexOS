# Target file name.
TARGET = libhalx86

# List C source files here.
CCSOURCES = 

# List C++ source files here.
CXXSOURCES = HAL.cpp \
             TextmodeDebugOutput.cpp \
             SerialDebugOutput.cpp \
             x86Paging.cpp \
             GDT.cpp \
             IDT.cpp \
             x86InterruptDispatcher.cpp \
             BitfieldPhysicalMemoryAllocator.cpp \
             x86BootEnvironment.cpp

# NASM flags
NASMFLAGS = -f win32

# GAS flags
GASFLAGS = 

# List Files to be assembled here
NASMSOURCES = start.asm gdt_flush.asm interruptStubs.asm
GASSOURCES = 

# C compiler flags
CFLAGS =

# C++ compiler flags
CXXFLAGS  = -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs -mno-ms-bitfields
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long

# Additional include paths to consider
INCLUDES =	$(ROOT)/kernel/libhalx86/include \
            $(ROOT)/kernel/libhalinterface/include \
            $(ROOT)/kernel/libkcpp/include \
            $(ROOT)/kernel/kernel/include

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := 

include $(ROOT)/build/static-lib.mak
