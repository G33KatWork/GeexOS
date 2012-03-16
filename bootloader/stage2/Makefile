SRC                 := ../..
include $(SRC)/build/base.mak

SUBTARGETS = stage2 stage15
DIRS = $(SUBTARGETS:%=%.dir)

# Main targets
all: $(DIRS) stage2.bin
	@

stage2.bin: stage15/stage15.bin
	$(Q)cp $< $@

%.dir:
	$(call cmd_msg,SUBDIR,$*)
	$(call call_submake,$*,all)

# Cleaning
clean:
	@-for i in $(SUBTARGETS); do \
		$(MAKE) -C $$i clean; \
	done
	$(Q)rm -f stage2.bin

distclean: clean
	@-for i in $(SUBTARGETS); do \
		$(MAKE) -C $$i distclean; \
	done

.PHONY: all clean distclean