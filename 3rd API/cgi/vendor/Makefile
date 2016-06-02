.PHONY: clean all install check testlib

topdir := $(shell pwd)/../..
include $(topdir)/config.mk

SUBDIRS	:= tyco

all: compile

compile:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT)) || exit 1 ;\
	done

clean:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) \
	done

install:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) || exit 1 ;\
	done

check:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) || exit 1 ;\
	done

testlib:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) || exit 1 ;\
	done