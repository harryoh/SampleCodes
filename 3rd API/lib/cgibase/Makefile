.SUFFIXES: .o .c .cpp
.PHONY: clean all dep install check test testlib

topdir := $(shell pwd)/../..
include $(topdir)/config.mk

CFLAGS   :=
CFLAGS   += $(DEBUG_FLAGS)

INCLUDES := -I.
INCLUDES += -I$(topdir)/include
INCLUDES += -I$(topdir)/ext/qdecoder/lastest/source/src

SOURCES  := $(patsubst unit_test.cpp,,$(wildcard *.c *.cpp))
ALLOBJS  := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJS     := $(patsubst unit_test%,,$(patsubst example%,,$(ALLOBJS)))

QDECODER := $(topdir)/ext/qdecoder/lastest/install/
LIBS     := $(QDECODER)/lib/internal.o
LIBS     += $(QDECODER)/lib/qcgireq.o
LIBS     += $(QDECODER)/lib/qcgisess.o
LIBS     += $(QDECODER)/lib/qcgires.o
LIBS     += $(QDECODER)/lib/qentry.o

TESTLIBS := $(topdir)/test/libunit_test.a
TESTLIBS += -lcppunit

TARGET   := libcgibase.a
EXAMPLE  := example
TESTUNIT := unit_test
INSDIR   := $(rootfs_dir)/usr/www/iAPI
DEPEND   := .depend

all: dep lib

lib: $(OBJS)
	$(QUIETAR)$(AR) $(TOOLS_VERBOSE)cr $(TARGET) $(OBJS) $(LIBS)
	$(QUIETRANLIB)$(RANLIB) $(TARGET)

example:
	$(QUIETLD)$(CPP) $(CFLAGS) -o $(EXAMPLE).cgi $(EXAMPLE).cpp $(INCLUDES) \
	$(TARGET)

.c.o:
	$(QUIETCC)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@
	$(QUIET)[ -d testobjs ] || mkdir testobjs
	$(QUIET)cp $@ testobjs/

clean:
	$(QUIETRM)$(RM) -fR $(ALLOBJS) $(LIB) $(TARGET) $(EXAMPLE).cgi \
	$(DEPEND) $(TESTUNIT) $(TESTUNIT).o *.xml *.dSYM testobjs

dep: $(SOURCES)
	$(QUIETDP)$(CPP) $(INCLUDES) -MM $^ > $(DEPEND)

check: cpplint cppcheck

cppcheck:
	$(QUIET)hash cppcheck 2>&- || (echo "cppcheck is not installed!!"&&exit 1)
	$(QUIET)cppcheck $(INCLUDES) $(QUIETCPPCHECK) --enable=all --inconclusive \
	--std=posix --check-config $(SOURCES) || exit 1

cpplint:
	$(QUIET)hash python 2>&- || (echo "python is not installed!!" && exit 1)
	$(QUIET)python $(topdir)/tools/cpplint/cpplint.py \
		$(wildcard *.cpp *.hpp *.h)

test: testlib
	$(QUIETLD)$(CPP) $(CFLAGS) -o $(TESTUNIT) $(INCLUDES) $(TESTUNIT).o \
		$(topdir)/test/main.cpp $(TARGET) $(TESTLIBS)
	$(QUIET)./$(TESTUNIT) || exit 1

testlib: all
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) \
		-c $(TESTUNIT).cpp -o $(TESTUNIT).o
	$(QUIET)cp $(TESTUNIT).o testobjs/

install:

-include $(DEPEND)
