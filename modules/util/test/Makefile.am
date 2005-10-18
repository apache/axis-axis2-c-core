prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_util.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_util_la_SOURCES = util_hash_test.c util_stream_test.c util_log_test.c util_error_test.c util_test.c
include_HEADERS=$(top_builddir)/modules/util/test/*.h

libtest_util_la_LIBADD = $(LDFLAGS) $(top_builddir)/modules/util/src/libutil.la
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include

prgbin_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =  $(LDFLAGS) libtest_util.la  $(top_builddir)/modules/util/src/libutil.la

