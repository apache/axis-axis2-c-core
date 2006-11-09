bindir=$(prefix)/bin/system_test
TESTS = test_context
check_PROGRAMS = test_context
bin_PROGRAMS = test_context
SUBDIRS =
AM_CFLAGS = -g -pthread
test_context_SOURCES = test_context.c


test_context_LDADD   =	../../../util/src/libaxis2_util.la \
						../../../axiom/src/om/libaxis2_axiom.la \
						../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
						../../../woden/src/wsdl20/libwoden.la \
						../../../xml_schema/src/libaxis2_xml_schema.la \
						$(top_builddir)/modules/core/engine/libaxis2_engine.la \
						$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
			        	-L$(top_builddir)$(GUTHTHILA_LIBS) \
			        	$(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/xml_schema/include \
			-I ../../../util/include \
			-I ../../../axiom/include 
