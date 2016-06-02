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

SOURCES  := $(patsubst unit_test.cpp,,$(wildcard *.c *.cpp))
ALLOBJS  := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJS     := $(patsubst unit_test%,,$(ALLOBJS))

LIBS     := $(topdir)/lib/cgibase/libcgibase.a
LIBS     += $(topdir)/cgi/vendor/tyco/libtycocgi.a

TESTLIBS = $(LIBS)
TESTLIBS += $(topdir)/test/libunit_test.a
TESTLIBS += -lcppunit
TESTOBJS := $(patsubst main%,,$(OBJS))

TARGET   := hello.cgi
TESTUNIT := unit_test
DEPEND   := .depend
INSDIR   := $(rootfs_dir)/usr/www/iAPI

all : dep $(TARGET)

$(TARGET): $(OBJS)
	$(QUIETLD)$(CPP) -o $@ $^ $(LDFLAGS) $(LIBS)

.c.o:
	$(QUIETCPP)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@
	$(QUIET)[ -d testobjs ] || mkdir testobjs
	$(QUIET)cp $@ testobjs/

clean:
	$(QUIETRM)$(RM) -rf $(ALLOBJS) $(TARGET) $(DEPEND) $(TESTUNIT) \
		$(TESTUNIT).o $(TESTUNIT).a *.dSYM *.xml testobjs

dep: $(SOURCES)
	$(QUIETDP)$(CPP) $(INCLUDES) -MM $^ > $(DEPEND)

check: cpplint cppcheck

cppcheck:
	$(QUIET)hash cppcheck 2>&- || (echo "cppcheck is not installed!!"&&exit 1)
	$(QUIET)cppcheck $(INCLUDES) \
		$(QUIETCPPCHECK) --enable=all --inconclusive --std=posix \
		--check-config $(SOURCES) || exit 1

cpplint:
	$(QUIET)hash python 2>&- || (echo "python is not installed!!" && exit 1)
	$(QUIET)python $(topdir)/tools/cpplint/cpplint.py \
		$(wildcard *.cpp *.hpp *.h)

test: testlib
	$(QUIETLD)$(CPP) $(CFLAGS) -o $(TESTUNIT) $(INCLUDES) $(TESTUNIT).o \
                $(topdir)/test/main.cpp $(TESTOBJS) $(TESTLIBS)
	$(QUIET)./$(TESTUNIT) || exit 1

testlib:
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) \
		-c $(TESTUNIT).cpp -o $(TESTUNIT).o
	$(QUIET)cp $(TESTUNIT).o testobjs/
	$(QUIET)rm -f testobjs/main.o

install:
	$(QUIET)[ -d "$(INSDIR)" ] || mkdir -p $(INSDIR)
#	$(QUIET)install -D -s --strip-program=$(STRIP) $(TARGET) $(INSDIR)/$(TARGET)
	$(QUIET)cp $(TARGET) $(INSDIR)
	$(QUIET)$(STRIP) $(INSDIR)/$(TARGET)

-include $(DEPEND)
