prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =  $(LDFLAGS) \
                    $(top_builddir)/modules/common/test/libtest_common.la \
                    $(top_builddir)/modules/util/test/libtest_util.la \
                    $(top_builddir)/modules/common/src/libaxis2_common.la \
                    $(top_builddir)/modules/util/src/libaxis2_util.la

INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include
