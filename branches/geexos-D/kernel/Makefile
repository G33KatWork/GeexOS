SRC                 := ..
include $(SRC)/build/base.mak

# Target file name.
TARGET = kernel

# List C source files here.
CCSOURCES = src/lib/string.c

# List C++ source files here
CXXSOURCES =

# List D source files here
DSOURCES = 

# List Objective-C source files here
OBJCSOURCES = src/main.m

# List Assembler to be assembled with NASM here
ASOURCES =	arch/current/src/start.S

# Addiotional include paths to consider
INCLUDES =	include include/lib include/runtime

# C compiler flags
CFLAGS += -g -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=gnu99
CFLAGS += -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
			-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
			-Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
			-Wstrict-prototypes #-Wconversion
CFLAGS += -DARCH_X86 -D_DEBUG
CFLAGS += $(addprefix -I,$(INCLUDES))

# C++ compiler flags
CXXFLAGS += -g -fno-builtin -fno-exceptions -nostdlib -nostartfiles -nodefaultlibs
CXXFLAGS += -Wall #-Wextra -Werror
CXXFLAGS += -DARCH_X86 -D_DEBUG #-fasm-blocks
CXXFLAGS += $(addprefix -I,$(INCLUDES))

# D compiler flags
DFLAGS += -nostdlib -nodefaultlibs -g #-mcmodel=kernel
DFLAGS += $(addprefix -I,$(INCLUDES))

# Objective-C compiler flags
OBJCFLAGS := $(CFLAGS)

# NASM flags
ASFLAGS = -f elf -g

# Linker flags
LDFLAGS = -T linker.ld

# Determinte objects to be created
OBJECTS += $(ASOURCES:.S=.o)
OBJECTS += $(CXXSOURCES:.cpp=.o)
OBJECTS += $(CCSOURCES:.c=.o)
OBJECTS += $(DSOURCES:.d=.o)
OBJECTS += $(OBJCSOURCES:.m=.o)

# Main targets
all: $(TARGET).elf
	@:	#Shut up!

$(TARGET).elf: $(OBJECTS)
	$(call cmd_msg,LINK,$(@))
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(call cmd_msg,OBJDUMP,$(@) -> source)
	$(Q)$(OBJDUMP) --source $(TARGET).elf > source

# Cleaning
clean:
	$(Q)rm -f $(foreach ext,elf,$(TARGET).$(ext)) \
	$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o,$(file).$(ext)))
	$(Q)rm -f source

distclean: clean

# Compile cpp files
%.o: %.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile c files
%.o: %.c
	$(call cmd_msg,CC,$<)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
	
# Compile d files
%.o: %.d
	$(call cmd_msg,DC,$<)
	$(Q)$(DC) $(DFLAGS) -c $< -o $@
	
# Compile Objective-C files
%.o: %.m
	$(call cmd_msg,OBJCC,$<)
	$(Q)$(CC) $(OBJCFLAGS) -c $< -o $@

# Assembler S files
%.o: %.S
	$(call cmd_msg,NASM,$<)
	$(Q)$(NASM) $(ASFLAGS) -o $@ $<

.PHONY: clean distclean

