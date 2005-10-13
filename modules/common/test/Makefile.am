lib_LTLIBRARIES = libtest_common.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_common_la_SOURCES = common_test.c

libtest_common_la_LIBADD = $(LDFLAGS) ../src/libcommon.la -ldl
INCLUDES = -I../../../include -I${CUTEST_HOME}/include

bin_PROGRAMS = common_test
common_test_SOURCES = main.c
common_test_LDADD   =  $(LDFLAGS) libtest_common.la ../src/libcommon.la -ldl

