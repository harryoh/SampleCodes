.SUFFIXES: .o .c .cpp
.PHONY: clean all dep install check testlib

topdir := $(shell pwd)/../../..
include $(topdir)/config.mk

CFLAGS :=
CFLAGS += $(DEBUG_FLAGS)

INCLUDES := -I.
INCLUDES += -I$(topdir)/include
INCLUDES += -I$(topdir)/lib/cgibase
INCLUDES += -I$(topdir)/cgi/vendor/tyco

SOURCES  := $(wildcard *.c *.cpp)
ALLOBJS  := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJS     := $(patsubst example%,,$(ALLOBJS))

LIBS     := $(topdir)/lib/cgibase/libcgibase.a
LIBS     += $(topdir)/cgi/vendor/tyco/libtycocgi.a

TARGET   := reboot.cgi
DEPEND   := .depend
INSDIR   := $(rootfs_dir)/usr/www/iAPI

all : dep $(TARGET)

$(TARGET): $(OBJS)
	$(QUIETLD)$(CPP) -o $@ $^ $(LDFLAGS) $(LIBS)

.c.o:
	$(QUIETCPP)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(QUIETRM)$(RM) $(ALLOBJS) $(TARGET) $(DEPEND)

dep: $(SOURCES)
	$(QUIETDP)$(CPP) $(INCLUDES) -MM $^ > $(DEPEND)

check: cpplint cppcheck

cppcheck:
	$(QUIET)hash cppcheck 2>&- || (echo "cppcheck is not installed!!"&&exit 1)
	$(QUIET)cppcheck $(INCLUDES) -I/usr/local/include $(QUIETCPPCHECK) \
		--enable=all --inconclusive --std=posix --check-config \
		$(SOURCES) || exit 1

cpplint:
	$(QUIET)hash python 2>&- || (echo "python is not installed!!" && exit 1)
	$(QUIET)python $(topdir)/tools/cpplint/cpplint.py $(wildcard *.cpp *.hpp *.h)

install:
	$(QUIET)[ -d "$(INSDIR)" ] || mkdir -p $(INSDIR)
#	$(QUIET)install -D -s --strip-program=$(STRIP) $(TARGET) $(INSDIR)/$(TARGET)
	$(QUIET)cp $(TARGET) $(INSDIR)
	$(QUIET)$(STRIP) $(INSDIR)/$(TARGET)

testlib:

-include $(DEPEND)
