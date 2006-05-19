TESTS = om_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS = om_node_test.h $(top_builddir)/test/unit/om/*.h
prglib_LTLIBRARIES = libtest_om.la
libtest_om_la_SOURCES = om_test.c om_node_test.c

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/src/om \
			@UTILINC@ \
			@UTILINC@/platforms

check_PROGRAMS = om_test
prgbin_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =   libtest_om.la  \
                    -L$(CUTEST_HOME)/lib \
                    -lcutest \
		            -laxis2_util \
					-laxis2_unix \
					$(top_builddir)/src/om/libaxis2_om.la \
                    $(top_builddir)/src/parser/libxml2/libaxis2_parser.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
