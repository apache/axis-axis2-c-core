prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_engine.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_engine_la_SOURCES = engine_test.c
include_HEADERS=$(top_builddir)/modules/core/engine/test/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = engine_test
engine_test_SOURCES = main.c
engine_test_LDADD   =  $(LDFLAGS) libtest_engine.la  \
                    -L$(top_builddir)/lib \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_guththila \
                    -lguththila
