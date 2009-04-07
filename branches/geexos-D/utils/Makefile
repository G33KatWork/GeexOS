SRC := ..
include $(SRC)/build/base.mak

all: geninitramfs

clean:
	$(Q)rm -f geninitramfs
	$(Q)rm -f initrd.img

%: %.c
	$(call cmd_msg,CC,$<)
	$(Q)$(HOSTCC) $(HOSTCFLAGS) $< -o $@

.PHONY: clean
.DEFAULT_GOAL = all
