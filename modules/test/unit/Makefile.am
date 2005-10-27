prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =  $(LDFLAGS) \
                    -L$(top_builddir)/modules/common/test/.libs -ltest_common \
                    -L$(top_builddir)/modules/util/test/.libs -ltest_util \
                    -L$(top_builddir)/modules/xml/om/test/.libs -ltest_om \
                    -L$(top_builddir)/modules/xml/guththila/src/.libs -lguththila

INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include
