prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_description_la_SOURCES = description_test.c
include_HEADERS=$(top_builddir)/modules/core/description/test/*.h

libtest_description_la_LIBADD = $(LDFLAGS) -L$(top_builddir)/modules/core/description/src/.libs \
                                -laxis2_description
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include

prgbin_PROGRAMS = description_test
description_test_SOURCES = main.c
description_test_LDADD   =  $(LDFLAGS) libtest_description.la  \
                    -L$(top_builddir)/modules/core/description/src/.libs -laxis2_description \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util

