prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_wsdl_la_SOURCES =
include_HEADERS=$(top_builddir)/modules/wsdl/test/*.h

libtest_wsdl_la_LIBADD =  -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include

prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c
wsdl_test_LDADD   =   libtest_wsdl.la  \
                    -L$(top_builddir)/modules/wsdl/src/.libs -laxis2_wsdl \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util

