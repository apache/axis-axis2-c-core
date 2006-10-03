TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = core wsdl
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/test/unit/core/description -ltest_description \
                    -L$(top_builddir)/test/unit/core/engine -ltest_engine \
                    -L$(top_builddir)/test/unit/core/context -ltest_context \
                    -L$(top_builddir)/test/unit/core/phaseresolver -ltest_phaseresolver \
                    -L$(top_builddir)/test/unit/wsdl -ltest_wsdl \
					../../util/src/libaxis2_util.la \
					../../axiom/src/om/libaxis2_axiom.la \
					../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					../../woden/src/wsdl20/libwoden.la \
					../../xml_schema/src/libaxis2_xml_schema.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
					$(top_builddir)/modules/core/transport/http/receiver/libaxis2_http_receiver.la \
                    -L$(top_builddir)$(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/xml_schema/include \
			-I ../../util/include \
			-I ../../axiom/include 
