TESTS = context_test
bindir=$(prefix)/bin/unit_test
libdir=$(prefix)/lib/unit_test
lib_LTLIBRARIES = libtest_context.la
libtest_context_la_SOURCES = context_test.c
noinst_HEADERS=$(top_builddir)/test/unit/core/context/*.h


INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
			-I ../../../../util/include \
			-I ../../../../axiom/include

bin_PROGRAMS = context_test
check_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =   \
                        libtest_context.la  \
                        -L$(CUTEST_HOME)/lib \
                        -lcutest \
						../../../../util/src/libaxis2_util.la \
						../../../../axiom/src/om/libaxis2_axiom.la \
						../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
						../../../../woden/src/wsdl20/libwoden.la \
						../../../../xml_schema/src/libaxis2_xml_schema.la \
						$(top_builddir)/modules/core/engine/libaxis2_engine.la \
						$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                        -L$(top_builddir)$(GUTHTHILA_LIBS) \
                        $(LIBXML2_LIBS)

