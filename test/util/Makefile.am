prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_util
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
test_util_SOURCES = test_util.c

test_util_LDADD   =   -L$(top_builddir)/install/lib -laxis2_util

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util
