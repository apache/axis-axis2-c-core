prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_common.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_common_la_SOURCES = common_test.c
include_HEADERS=$(top_builddir)/modules/common/test/*.h
libtest_common_la_LIBADD = $(LDFLAGS) \
                    $(top_builddir)/modules/common/src/libcommon.la
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include

prgbin_PROGRAMS = common_test
common_test_SOURCES = main.c
common_test_LDADD   =  $(LDFLAGS) libtest_common.la \
                    $(top_builddir)/modules/common/src/libcommon.la

