.PHONY: clean all install

topdir := $(shell pwd)/../..
include $(topdir)/config.mk

CONF := CC=$(CC) RANLIB=$(RANLIB) ./configure \
				--prefix=$(shell pwd)/lastest/install

ifeq ($(platform), arm_v5t)
	CONF += --host=arm-linux
endif

ifeq ($(debug), y)
	CONF += --enable-debug
endif

installed := $(wildcard .compiled)

ifneq ($(verbose), 2)
	output := > /dev/null 2>&1
endif

all:  compile

compile:
	$(QUIET)[ -f "$(installed)" ] \
	&& ( \
		echo "Already qdecoder is compiled!!" $(output) && exit 0 ; ) \
	|| ( \
		cd lastest/source;$(CONF) $(output) && \
		$(MAKE) $(SILENT) all install $(output);\
		cd ../.. && touch .compiled && \
		cp -f lastest/source/src/*.o lastest/install/lib ; )

clean:
	$(QUIET)cd lastest/source; $(MAKE) $(SILENT) clean $(output); cd ..
	$(QUIET)$(RM) -rf lastest/install/lib/*
	$(QUIET)$(RM) -rf lastest/install/include/*
	$(QUIET)$(RM) -rf .compiled

install:
