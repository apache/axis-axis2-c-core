prgbindir=$(prefix)/bin/system_test/
TESTS = test_client test_clientapi
prgbin_PROGRAMS = test_client test_clientapi
check_PROGRAMS = test_client test_clientapi
SUBDIRS =
AM_CFLAGS = -g -pthread 
test_client_SOURCES = test_client.c
test_clientapi_SOURCES = test_clientapi.c

test_clientapi_LDADD   =  \
                    ../../../util/src/libaxis2_util.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
                    ../../../woden/src/wsdl20/libwoden.la \
                    ../../../xml_schema/src/libaxis2_xml_schema.la \
                    -lpthread \
                    $(top_builddir)/modules/core/engine/libaxis2_engine.la \
                    $(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)

test_client_LDADD   =  \
                    ../../../util/src/libaxis2_util.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
                    ../../../woden/src/wsdl20/libwoden.la \
                    ../../../xml_schema/src/libaxis2_xml_schema.la \
                    -lpthread \
                    $(top_builddir)/modules/core/engine/libaxis2_engine.la \
                    $(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS) 

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/xml_schema/include \
            -I ../../../util/include \
            -I ../../../axiom/include

