TESTS = wsdl_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la

libtest_wsdl_la_SOURCES = wsdl_test.c test_wsdl_component.c

noinst_HEADERS=$(top_builddir)/test/unit/wsdl/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/platforms


check_PROGRAMS = wsdl_test
prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c

wsdl_test_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     $(top_builddir)/test/unit/wsdl/libtest_wsdl.la \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_wsdl \
					-laxis2_unix \
                    -laxis2_engine \
                    -laxis2_om \
                    -laxis2_soap \
                    -laxis2_parser \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)
