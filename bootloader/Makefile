SRC                 := ..
include $(SRC)/build/base.mak

SUBTARGETS = hello mbr stage1_fat16 stage2
DIRS = $(SUBTARGETS:%=%.dir)

# Main targets
all: $(DIRS)
	@

%.dir:
	$(call cmd_msg,SUBDIR,$*)
	$(call call_submake,$*,all)

# Cleaning
clean:
	@-for i in $(SUBTARGETS); do \
		$(MAKE) -C $$i clean; \
	done

distclean: clean
	@-for i in $(SUBTARGETS); do \
		$(MAKE) -C $$i distclean; \
	done

.PHONY: all clean distclean