SRC := ..
include $(SRC)/build/base.mak

all: geninitramfs genmapfile

clean:
	$(Q)rm -f geninitramfs
	$(Q)rm -f genmapfile
	$(Q)rm -f initrd.img

%: %.c
	$(call cmd_msg,CC,$<)
	$(Q)$(HOSTCC) $(HOSTCFLAGS) $< -o $@

%: %.cpp
	$(call cmd_msg,CXX,$<)
	$(Q)$(HOSTCXX) $(HOSTCXXFLAGS) $< -o $@

.PHONY: clean
.DEFAULT_GOAL = all
