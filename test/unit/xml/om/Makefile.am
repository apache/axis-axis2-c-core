TESTS = om_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS = om_node_test.h $(top_builddir)/test/unit/xml/om/*.h
prglib_LTLIBRARIES = libtest_om.la
libtest_om_la_SOURCES = om_test.c om_node_test.c

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/om

check_PROGRAMS = om_test
prgbin_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =   libtest_om.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -laxis2_util \
                    -laxis2_om \
                    -laxis2_unix \
                    -laxis2_parser \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
