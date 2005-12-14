prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_phaseresolver.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_phaseresolver_la_SOURCES = phaseresolver_test.c \
                                    test_resolver.c

include_HEADERS=$(top_builddir)/test/unit/core/phaseresolver/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/wsdl/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = phaseresolver_test
phaseresolver_test_SOURCES = main.c
phaseresolver_test_LDADD   =  $(LDFLAGS) libtest_phaseresolver.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

