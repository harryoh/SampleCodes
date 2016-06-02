# You can change default value
#verbose := 0
#debug := y
#platform := arm_v5t
#rootfs_dir := /Users/harry/rootfs/
#THREAD     = -j4

# Directories
ifndef rootfs_dir
  rootfs_dir := $(topdir)/../../../build/rootfs
endif

# Tools
cpplint := python $(topdir)/tools/cpplint/cpplint.py

# Choice platform
ifeq ($(platform), arm_v5t)
    CROSS_COMPILE := arm_v5t_le-
else
    CROSS_COMPILE :=
    ifndef ($platform)
        platform := $(shell if [ `getconf LONG_BIT` = "64" ]; then echo "x64";\
        else echo "x86"; fi)
    endif
endif

CC      = $(CROSS_COMPILE)gcc
CPP     = $(CROSS_COMPILE)g++
AR      = $(CROSS_COMPILE)ar
STRIP   = $(CROSS_COMPILE)strip
RANLIB  = $(CROSS_COMPILE)ranlib
RM      = rm -f

# Debug Mode
ifeq ($(debug), y)
  DEBUG_FLAGS   := -g -D_DEBUG
  verbose       := 2
else
  DEBUG_FLAGS   := -O2 -Wall
endif

# default verbose
ifndef verbose
  verbose := 0
endif

ifeq ($(verbose), 2)
  QUIET         =
  SILENT        =
endif
ifeq ($(verbose), 1)
  QUIETDP       = @echo "[DEPEND]" $@;
  QUIETCC       = @echo "[CC]" $@;
  QUIETCPP      = @echo "[CPP]" $@;
  QUIETLD       = @echo "[LD]" $@;
  QUIETAR       = @echo "[AR]" $@;
  QUIETRANLIB   = @echo "[RANLIB]" $@;
  QUIETRM       = @echo "[RM]" $@;
  QUIETCPPCHECK = --quiet
endif
ifeq ($(verbose), 0)
  QUIET         = @
  SILENT        = -s
  QUIETCPPCHECK = --quiet
endif
