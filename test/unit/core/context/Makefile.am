prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_context.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_context_la_SOURCES = context_test.c
include_HEADERS=$(top_builddir)/test/unit/core/context/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =  $(LDFLAGS) libtest_context.la  \
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
                    -laxis2_guththila \
                    -lguththila

