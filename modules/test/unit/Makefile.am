prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util core xml
AM_CPPFLAGS = $(CPPFLAGS)
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =  $(LDFLAGS) \
                    -L$(top_builddir)/modules/common/test/.libs -ltest_common \
                    -L$(top_builddir)/modules/util/test/.libs -ltest_util \
                    -L$(top_builddir)/modules/xml/om/test/.libs -ltest_om \
                    -L$(top_builddir)/modules/xml/parser/guththila/src/.libs -laxis2_guththila \
                    -L$(top_builddir)/modules/xml/parser/guththila/impl/src/.libs -lguththila

INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src
