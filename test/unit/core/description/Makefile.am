TESTS = description_test
bindir=$(prefix)/bin/unit_test
libdir=$(prefix)/lib/unit_test
lib_LTLIBRARIES = libtest_description.la
libtest_description_la_SOURCES = description_test.c \
                                    test_op.c \
                                    test_svc.c \
                                    test_flow.c \
                                    test_handler_desc.c

noinst_HEADERS=$(top_builddir)/test/unit/core/description/*.h \
                $(top_builddir)/test/unit/core/phaseresolver/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I${CUTEST_HOME}/include \
           -I$(top_builddir)/xml_schema/include\
		   -I ../../../../util/include \
		   -I ../../../../axiom/include 

bin_PROGRAMS = description_test
check_PROGRAMS = description_test
description_test_SOURCES = main.c
description_test_LDADD   =   libtest_description.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../util/src/libaxis2_util.la \
					 ../../../../axiom/src/om/libaxis2_axiom.la \
					 ../../../../xml_schema/src/libaxis2_xml_schema.la \
					 ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					 ../../../../woden/src/wsdl20/libwoden.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    -L$(top_builddir)$(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

