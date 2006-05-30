TESTS = wsdl_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la

libtest_wsdl_la_SOURCES = wsdl_test.c test_wsdl_component.c

noinst_HEADERS=$(top_builddir)/test/unit/wsdl/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
			@AXIOMINC@ \
			@UTILINC@ 


check_PROGRAMS = wsdl_test
prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c

wsdl_test_LDADD   =   \
                    -L$(CUTEST_HOME)/lib \
                    -lcutest \
                    $(top_builddir)/test/unit/wsdl/libtest_wsdl.la \
                    -laxis2_util \
					-laxis2_axiom \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
