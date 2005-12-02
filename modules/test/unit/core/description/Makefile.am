prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_description_la_SOURCES = description_test.c
include_HEADERS=$(top_builddir)/modules/core/description/test/*.h

libtest_description_la_LIBADD = $(LDFLAGS) \
                        -L$(top_builddir)/modules/core/description/src/.libs -laxis2_description \
                        -L$(top_builddir)/modules/core/engine/src/.libs -laxis2_engine \
                        -L$(top_builddir)/modules/core/context/src/.libs -laxis2_context \
                        -L$(top_builddir)/modules/core/phaseresolver/src/.libs -laxis2_phaseresolver

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = description_test
description_test_SOURCES = main.c
description_test_LDADD   =  $(LDFLAGS) libtest_description.la  \
                    -L$(top_builddir)/modules/core/description/src/.libs -laxis2_description \
                    -L$(top_builddir)/modules/core/engine/src/.libs -laxis2_engine \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util \
                    -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl \
                    -L$(top_builddir)/modules/core/phaseresolver/src/.libs -laxis2_phaseresolver

