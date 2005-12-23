prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_context.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_context_la_SOURCES = context_test.c
include_HEADERS=$(top_builddir)/modules/core/context/test/*.h

libtest_context_la_LIBADD =  -L$(top_builddir)/modules/core/context/src/.libs \
                                -laxis2_context
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =   libtest_context.la  \
                    -L$(top_builddir)/modules/core/context/src/.libs -laxis2_context \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util \
                    -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl \
                    -L$(top_builddir)/modules/core/description/src/.libs -laxis2_description

