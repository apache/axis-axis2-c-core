prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_om_la_SOURCES = om_test.c
include_HEADERS=$(top_builddir)/modules/xml/om/test/*.h

libtest_om_la_LIBADD = $(LDFLAGS) -L$(top_builddir)/modules/xml/om/src/.libs -laxis2_om
INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src

prgbin_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =  $(LDFLAGS) libtest_om.la  \
                   -L$(top_builddir)/modules/xml/om/src/.libs -laxis2_om \
                   -L$(top_builddir)/modules/xml/guththila/src/.libs -lguththila \
                   -L$(top_builddir)/modules/util/src/.libs -laxis2_util

