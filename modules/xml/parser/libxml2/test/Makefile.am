prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_libxml2.la
AM_CPPFLAGS = $(CPPFLAGS) -g
libtest_libxml2_la_SOURCES = libxml2_test.c
include_HEADERS=$(top_builddir)/modules/xml/parser/libxml2/test/*.h

libtest_libxml2_la_LIBADD = $(LDFLAGS) $(top_builddir)/modules/libxml2/src/libaxis2_libxml2.la
INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = libxml2_test
libxml2_test_SOURCES = main.c
libxml2_test_LDADD   =  $(LDFLAGS) libtest_libxml2.la  $(top_builddir)/modules/libxml2/src/libaxis2_libxml2.la

