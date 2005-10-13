lib_LTLIBRARIES = libtest_util.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_util_la_SOURCES = util_hash_test.c util_stream_test.c util_log_test.c util_error_test.c util_test.c

libtest_util_la_LIBADD = $(LDFLAGS) ../src/libutil.la -ldl
INCLUDES = -I../../../include -I${CUTEST_HOME}/include

bin_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =  $(LDFLAGS) libtest_util.la ../src/libutil.la -ldl

