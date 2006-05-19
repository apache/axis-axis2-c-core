TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = om
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 -laxis2_util \
					 -laxis2_unix \
					 -lxml2 \
                    -L$(top_builddir)/test/unit/om -ltest_om \
                    $(top_builddir)/src/om/libaxis2_om.la \
                    $(top_builddir)/src/parser/libxml2/libaxis2_libxml2.la
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include
