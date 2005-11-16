prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_guththila.la
AM_CPPFLAGS = $(CPPFLAGS) -g
libtest_guththila_la_SOURCES = guththila_test.c

libtest_guththila_la_LIBADD = $(LDFLAGS) $(top_builddir)/modules/xml/parser/guththila/src/libaxis2_guththila.la
INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = guththila_test
guththila_test_SOURCES = main.c
guththila_test_LDADD   =  $(LDFLAGS) libtest_guththila.la  $(top_builddir)/modules/xml/parser/guththila/src/libaxis2_guththila.la

