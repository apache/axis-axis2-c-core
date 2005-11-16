prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_engine.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_engine_la_SOURCES = engine_test.c
include_HEADERS=$(top_builddir)/modules/core/engine/test/*.h

libtest_engine_la_LIBADD = $(LDFLAGS) -L$(top_builddir)/modules/core/engine/src/.libs \
                                -laxis2_engine
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = engine_test
engine_test_SOURCES = main.c
engine_test_LDADD   =  $(LDFLAGS) libtest_engine.la  \
                    -L$(top_builddir)/modules/core/engine/src/.libs -laxis2_engine \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util \
                    -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl \
                    -L$(top_builddir)/modules/description/src/.libs -laxis2_description

