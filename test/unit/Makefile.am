prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util core xml
AM_CPPFLAGS = $(CPPFLAGS)
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =  $(LDFLAGS) \
                    -L$(top_builddir)/test/test/unit/core/description/.libs -ltest_description \
                    -L$(top_builddir)/test/test/unit/core/engine/.libs -ltest_engine \
                    -L$(top_builddir)/test/test/unit/core/context/.libs -ltest_context \
                    -L$(top_builddir)/test/test/unit/core/phaseresolver/.libs -ltest_phaseresolver \
                    -L$(top_builddir)/test/unit/util/.libs -ltest_util \
                    -L$(top_builddir)/test/unit/xml/om/.libs -ltest_om \
                    -L$(top_builddir)/install/lib -laxis2_guththila \
                    -L$(top_builddir)/install/lib -lguththila

INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src
