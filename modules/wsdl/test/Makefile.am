prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_wsdl_la_SOURCES =
include_HEADERS=$(top_builddir)/modules/wsdl/test/*.h

libtest_wsdl_la_LIBADD = $(LDFLAGS) $(top_builddir)/modules/wsdl/src/libaxis2_wsdl.la
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include

prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c
wsdl_test_LDADD   =  $(LDFLAGS) libtest_wsdl.la  $(top_builddir)/modules/wsdl/src/libaxis2_wsdl.la

