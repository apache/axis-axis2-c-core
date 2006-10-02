TESTS = wsdl_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la

libtest_wsdl_la_SOURCES = wsdl_test.c test_wsdl_component.c

noinst_HEADERS=$(top_builddir)/test/unit/wsdl/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/xml_schema/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
			-I ../../../util/include \
			-I ../../../axiom/include 


check_PROGRAMS = wsdl_test
prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c

wsdl_test_LDADD   =   \
                    -L$(CUTEST_HOME)/lib \
                    -lcutest \
                    $(top_builddir)/test/unit/wsdl/libtest_wsdl.la \
					../../../util/src/libaxis2_util.la \
					../../../axiom/src/om/libaxis2_axiom.la \
					../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					../../../woden/src/wsdl20/libwoden.la \
					../../../xml_schema/src/libaxis2_xml_schema.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
