prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_om_la_SOURCES = om_test.c
include_HEADERS=$(top_builddir)/modules/om/test/*.h

libtest_om_la_LIBADD = $(LDFLAGS) $(top_builddir)/modules/om/src/libaxis2_om.la
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include

prgbin_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =  $(LDFLAGS) libtest_om.la  $(top_builddir)/modules/om/src/libaxis2_om.la

