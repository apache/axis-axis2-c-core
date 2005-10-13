bin_PROGRAMS = unit_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
unit_test_SOURCES = main.c ../../common/test/common_test.c

unit_test_LDADD   =  $(LDFLAGS) ../../common/src/libcommon.la -ldl
INCLUDES = -I${CUTEST_HOME}/include -I${APR_HOME}/include/apr-1 -I../../../include
