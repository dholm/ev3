export TOP_DIR := $(CURDIR)

include $(TOP_DIR)/Makefile.inc

SUBDIRS := src test

all: $(SUBDIRS)

$(SUBDIRS):
	$(Q)make -C $@ all

clean:
	$(Q)for dir in $(SUBDIRS); \
	do \
	  (make -C $$dir $@) || exit $$?; \
	done

.PHONY: $(SUBDIRS) clean
