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
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
