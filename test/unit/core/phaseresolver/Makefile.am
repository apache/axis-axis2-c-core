prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_phaseresolver.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_phaseresolver_la_SOURCES = phaseresolver_test.c
include_HEADERS=$(top_builddir)/modules/core/phaseresolver/test/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = phaseresolver_test
phaseresolver_test_SOURCES = main.c
phaseresolver_test_LDADD   =  $(LDFLAGS) libtest_phaseresolver.la  \
                    -L$(top_builddir)/install/lib \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_guththila \
                    -lguththila

