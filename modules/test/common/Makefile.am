prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_common
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
test_common_SOURCES = test_common.c

test_common_LDADD   =  $(LDFLAGS) ../../common/src/libaxis2_common.la
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include
