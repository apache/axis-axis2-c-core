bin_PROGRAMS = common_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
common_test_SOURCES = common_test.c main.c

common_test_LDADD   =  $(LDFLAGS) ../src/libcommon.la -ldl
INCLUDES = -I${CUTEST_HOME}/include -I${APR_HOME}/include/apr-1 -I../../../include
