TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/test/unit/util -ltest_util \
                    $(top_builddir)/src/libaxis2_util.la \
					$(top_builddir)/src/platforms/unix/libaxis2_unix.la
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/include/platforms
