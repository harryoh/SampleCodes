.PHONY: clean all install check test

topdir := $(shell pwd)
include $(topdir)/config.mk

SUBDIRS	= ext test lib cgi

DOTPATH = $(shell if hash which; then echo `which dot`; fi)

all: compile

compile:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		[ "$(platform)" = "arm_v5t" ] && [ "$${DIR}" = "test" ] && continue;\
		(cd $${DIR}; $(MAKE) $(THREAD) $(SILENT)) || exit 1 ;\
	done

clean:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) \
	done
	$(QUIET)$(RM) -rf doc/html doc/latex

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

test: testlib
	$(QUIET)(cd test; $(MAKE) $(SILENT) $@) || exit 1 ;

testlib:
	$(QUIET)for DIR in $(SUBDIRS); \
	do \
		(cd $${DIR}; $(MAKE) $(SILENT) $@) || exit 1 ;\
	done

docs:
	$(QUIET)hash doxygen 2>&- || (echo "doxygen is not installed!!" && exit 1)
	$(QUIET)cp doxygen/Doxyfile /tmp/.Doxyfile || exit 1
	$(QUIET)if [ -f "$(DOTPATH)" ]; then \
		echo "HAVE_DOT = YES" | cat >> /tmp/.Doxyfile;\
		echo "DOT_TRANSPARENT = YES" | cat >> /tmp/.Doxyfile; \
	fi

	$(QUIET)doxygen /tmp/.Doxyfile || (rm -f /tmp/.Doxyfile; exit 1;)
	$(QUIET)$(RM) -f /tmp/.Doxyfile
