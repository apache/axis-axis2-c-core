bin_PROGRAMS = unit_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
unit_test_SOURCES = main.c

unit_test_LDADD   =  $(LDFLAGS) ../../common/test/libtest_common.la ../../util/test/libtest_util.la ../../common/src/libcommon.la ../../util/src/libutil.la -ldl
INCLUDES = -I${CUTEST_HOME}/include -I../../../include
