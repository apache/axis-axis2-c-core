bin_PROGRAMS = util_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
util_test_SOURCES = main.c 
util_test_LDADD   =  $(LDFLAGS) libutil_test.la ../src/libutil.la ../../common/src/libcommon.la


noinst_LTLIBRARIES = libutil_test.la
libutil_test_la_SOURCES = util_stream_test.c util_log_test.c util_error_test.c util_hash_test.c util_test.c
libutil_test_LIBADD   =  $(LDFLAGS) ../src/libutil.la ../../common/src/libcommon.la
INCLUDES = -I${CUTEST_HOME}/include -I../../../include

