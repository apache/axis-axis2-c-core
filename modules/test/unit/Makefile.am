bin_PROGRAMS = unit_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
unit_test_SOURCES = main.c

unit_test_LDADD   =  $(LDFLAGS) ../../common/test/libcommon_test.la ../../util/test/libutil_test.la ../../common/src/libcommon.la ../../util/src/libutil.la -ldl
INCLUDES = -I${CUTEST_HOME}/include -I${APR_HOME}/include/apr-1 -I../../../include
