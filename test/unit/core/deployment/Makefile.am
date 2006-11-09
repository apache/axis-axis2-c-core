TESTS = deployment_test
bindir=$(prefix)/bin/unit_test
libdir=$(prefix)/lib/unit_test
lib_LTLIBRARIES = libtest_deployment.la
libtest_deployment_la_SOURCES = deployment_test.c \
                                test_phases_info.c \
                                test_dep_engine.c

noinst_HEADERS=$(top_builddir)/test/unit/core/deployment/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/xml_schema/include \
            -I${CUTEST_HOME}/include \
			-I ../../../../util/include \
			-I ../../../../axiom/include 

bin_PROGRAMS = deployment_test
check_PROGRAMS = deployment_test
deployment_test_SOURCES = main.c
deployment_test_LDADD   =   libtest_deployment.la  \
                    -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../util/src/libaxis2_util.la \
					 ../../../../axiom/src/om/libaxis2_axiom.la \
					 ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					 ../../../../woden/src/wsdl20/libwoden.la \
					 ../../../../xml_schema/src/libaxis2_xml_schema.la \
                    -lpthread \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    -L$(top_builddir)$(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

