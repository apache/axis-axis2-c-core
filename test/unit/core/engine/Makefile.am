TESTS = engine_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_engine.la
libtest_engine_la_SOURCES = engine_test.c test_conf.c
noinst_HEADERS=$(top_builddir)/test/unit/core/engine/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/wsdl \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/xml_schema/include \
			-I ../../../../util/include \
			-I ../../../../axiom/include 

check_PROGRAMS = engine_test
prgbin_PROGRAMS = engine_test
engine_test_SOURCES = main.c
engine_test_LDADD   =   libtest_engine.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../util/src/libaxis2_util.la \
					 ../../../../axiom/src/om/libaxis2_axiom.la \
					 ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					 ../../../../xml_schema/src/libaxis2_xml_schema.la \
					 ../../../../woden/src/wsdl20/libwoden.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
