prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_util
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
test_util_SOURCES = test_util.c

test_util_LDADD   =  $(LDFLAGS) $(top_builddir)/modules/util/src/libutil.la
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include
