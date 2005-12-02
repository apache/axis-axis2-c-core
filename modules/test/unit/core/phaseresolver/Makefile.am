prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_phaseresolver.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_phaseresolver_la_SOURCES = phaseresolver_test.c
include_HEADERS=$(top_builddir)/modules/core/phaseresolver/test/*.h

libtest_phaseresolver_la_LIBADD = $(LDFLAGS) -L$(top_builddir)/modules/core/phaseresolver/src/.libs \
                                -laxis2_phaseresolver
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = phaseresolver_test
phaseresolver_test_SOURCES = main.c
phaseresolver_test_LDADD   =  $(LDFLAGS) libtest_phaseresolver.la  \
                    -L$(top_builddir)/modules/core/phaseresolver/src/.libs -laxis2_phaseresolver \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util \
                    -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl

