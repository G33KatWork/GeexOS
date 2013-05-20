# Target file name.
TARGET = kernel

# List C source files here.
CCSOURCES = 

# List C++ source files here.
CXXSOURCES = main.cpp \
             global.cpp \
             Memory/PlacementAllocator.cpp \
             DataStructures/Bitfield.cpp \
             Memory/Virtual/VirtualMemoryManager.cpp \
             Memory/Virtual/VirtualMemoryRegion.cpp \
             Memory/Virtual/VirtualMemorySpace.cpp \
             Memory/Virtual/Regions/KernelProgramMemoryRegion.cpp \
             Memory/Virtual/Regions/BuddyAllocatedMemoryRegion.cpp \
             Memory/Slab/LargeCache.cpp \
             Memory/Slab/SlabAllocator.cpp \
             Memory/Slab/SlabCache.cpp \
             Memory/Slab/SmallCache.cpp \
             Memory/IO/IOMemoryManager.cpp \
             Objects/GXDirectoryObject.cpp \
             Objects/ObjectManager.cpp \
             Utils/CRC32.cpp

# List Files to be assembled here
NASMSOURCES = 
GASSOURCES = 

# C compiler flags
CFLAGS =

# C++ compiler flags
CXXFLAGS  = -ggdb -fno-builtin -fno-exceptions -fno-rtti -nostdlib -nostartfiles -nodefaultlibs -mno-ms-bitfields
CXXFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long -fvisibility=default

# NASM flags
NASMFLAGS = 

# GAS flags
GASFLAGS = 

# Linker flags
LDFLAGS = -T $(ROOT)/kernel/kernel/linker.ld -Map $(ROOT)/kernel/kernel/kernel.map \
          -entry _start --image-base 0xC0000000 --subsystem native \
          --disable-auto-import --exclude-all-symbols -nostdlib -pie

# Additional include paths to consider
INCLUDES =	$(ROOT)/kernel/kernel/include \
            $(ROOT)/kernel/libkcpp/include \
            $(ROOT)/kernel/libhalx86/include \
            $(ROOT)/kernel/libhalinterface/include

# Additional local static libs to link against
LIBS = $(BINARY-libhalinterface) $(BINARY-libhalx86) $(BINARY-libkcpp)

# Shared libraries
SODEPS = 

# Folder for object files
OBJDIR = obj

# Folder for sourcecode
SRCDIR = src

# Additional defines
DEFINES := -DDEBUG

include $(ROOT)/build/kernel-executable.mak
