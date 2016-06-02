.SUFFIXES: .o .c .cpp
.PHONY: clean all dep install check test testlib

topdir := $(shell pwd)/..
include $(topdir)/config.mk

CFLAGS   := $(DEBUG_FLAGS)

INCLUDES := -I.
INCLUDES += -I$(topdir)/include

SOURCES  := $(wildcard *.c *.cpp)
ALLOBJS  := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJS     := $(patsubst main%,,$(ALLOBJS))

LIBS     := -lcppunit
LIBS     += $(topdir)/lib/cgibase/libcgibase.a
LIBS     += $(topdir)/cgi/vendor/tyco/libtycocgi.a

TESTOBJS := $(shell find $(topdir) -name "*.o" | grep testobjs)

TARGET   := libunit_test.a
TESTUNIT := unit_test
DEPEND   := .depend

all: dep lib

lib: $(OBJS)
	$(QUIETAR)$(AR) $(TOOLS_VERBOSE)cr $(TARGET) $(OBJS)
	$(QUIETRANLIB)$(RANLIB) $(TARGET)

.c.o:
	$(QUIETCC)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(QUIETCPP)$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(QUIETRM)$(RM) -fR $(ALLOBJS) $(LIB) $(TARGET) $(TESTUNIT) $(DEPEND) \
		*.xml *.dSYM

dep: $(SOURCES)
	$(QUIETDP)$(CPP) $(CFLAGS) $(INCLUDES) -MM $^ > $(DEPEND)

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

test:
	$(QUIETLD)$(CPP) $(CFLAGS) $(INCLUDES) -o $(TESTUNIT) $(TESTOBJS) $(OBJS) \
                main.cpp $(LIBS)
	$(QUIET)./$(TESTUNIT) || exit 1

testlib:
install:

-include $(DEPEND)
