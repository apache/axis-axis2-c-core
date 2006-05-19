TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/test/unit/om -ltest_om \
                    $(top_builddir)/src/libaxis2_om.la
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include
